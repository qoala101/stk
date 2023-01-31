#include <absl/container/flat_hash_map.h>
#include <absl/hash/hash.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <boost/di.hpp>
#include <coroutine>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <exception>
#include <function2/function2.hpp>
#include <functional>
#include <memory>
#include <not_null.hpp>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "core_types.h"
#include "cpp_message_exception.h"
#include "cpp_not_null.h"
#include "gtest/gtest_pred_impl.h"
#include "network_auto_parsable.h"
#include "network_auto_parsable_request.h"
#include "network_endpoint_request_dispatcher.h"
#include "network_i_rest_request_handler.h"
#include "network_i_rest_request_receiver.h"
#include "network_i_rest_request_sender.h"
#include "network_json_base_conversions.h"
#include "network_json_common_conversions.h"
#include "network_json_conversions_facades.h"
#include "network_rest_client.h"
#include "network_rest_client_request_builder.h"
#include "network_rest_request_builder.h"
#include "network_rest_server.h"
#include "network_rest_server_builder.h"
#include "network_typed_endpoint.h"
#include "network_types.h"
#include "networkx_common.h"
#include "networkx_endpoint_function_traits_facade.h"
#include "networkx_types.h"
#include "test_restsdk_injector.h"

namespace {
const auto kBaseUri = vh::network::Uri{"http://0.0.0.0:30001/Entity"};
const auto kIndexOutOfBoundsMessage = "Index out of bounds";

class EntityInterface {
 public:
  virtual ~EntityInterface() = default;

  virtual auto PushSymbol(vh::stk::core::Symbol symbol) -> cppcoro::task<> = 0;

  virtual auto GetSymbol [[nodiscard]] (int index) const
      -> cppcoro::task<vh::stk::core::Symbol> = 0;

  virtual auto GetSize [[nodiscard]] () const -> cppcoro::task<int> = 0;
};

class Entity : public EntityInterface {
 public:
  auto PushSymbol(vh::stk::core::Symbol symbol) -> cppcoro::task<> override {
    symbols_.emplace_back(std::move(symbol));
    co_return;
  }

  auto GetSymbol [[nodiscard]] (int index) const
      -> cppcoro::task<vh::stk::core::Symbol> override {
    EXPECT_GT(symbols_.size(), index);
    co_return symbols_[index];
  }

  auto GetSize [[nodiscard]] () const -> cppcoro::task<int> override {
    co_return symbols_.size();
  }

 private:
  std::vector<vh::stk::core::Symbol> symbols_{};
};
}  // namespace

namespace vh::networkx {
template <>
struct EndpointFunctionTraits<&Entity::PushSymbol> {
  static constexpr auto kMethod = network::Method::kPost;
  static constexpr auto kParams = ParamList(RequestBody{});
};

template <>
struct EndpointFunctionTraits<&Entity::GetSymbol> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams = ParamList("index");
};

template <>
struct EndpointFunctionTraits<&Entity::GetSize> {
  static constexpr auto kMethod = network::Method::kGet;
};
}  // namespace vh::networkx

namespace {
class EntityServer {
 public:
  static auto PushSymbolEndpointDesc [[nodiscard]] ()
  -> vh::network::TypedEndpoint {
    return vh::networkx::EndpointFunctionTraitsFacade<
        &Entity::PushSymbol>::AsTypedEndpoint();
  }

  static auto GetSymbolEndpointDesc [[nodiscard]] ()
  -> vh::network::TypedEndpoint {
    return vh::networkx::EndpointFunctionTraitsFacade<
        &Entity::GetSymbol>::AsTypedEndpoint();
  }

  static auto GetSizeEndpointDesc [[nodiscard]] ()
  -> vh::network::TypedEndpoint {
    return vh::networkx::EndpointFunctionTraitsFacade<
        &Entity::GetSize>::AsTypedEndpoint();
  }

  explicit EntityServer(vh::network::Uri base_uri)
      : rest_server_{
            vh::network::RestServerBuilder{
                std::move(base_uri),
                test::restsdk::Injector()
                    .create<vh::cpp::NnUp<vh::network::IRestRequestReceiver>>()}
                .Handling(PushSymbolEndpointDesc(),
                          std::bind_front(
                              &EntityServer::PushSymbolEndpointHandler, this))
                .Handling(GetSymbolEndpointDesc(),
                          std::bind_front(
                              &EntityServer::GetSymbolEndpointHandler, this))
                .Handling(GetSizeEndpointDesc(),
                          std::bind_front(&EntityServer::GetSizeEndpointHandler,
                                          this))
                .Start()} {}

 private:
  auto PushSymbolEndpointHandler(vh::network::AutoParsableRestRequest request)
      -> cppcoro::task<> {
    return entity_.PushSymbol(request.Body());
  }

  auto GetSymbolEndpointHandler(vh::network::AutoParsableRestRequest request)
      -> cppcoro::task<vh::stk::core::Symbol> {
    const auto index = int{request.Param("index")};

    if (index >= co_await entity_.GetSize()) {
      throw std::runtime_error{kIndexOutOfBoundsMessage};
    }

    co_return co_await entity_.GetSymbol(index);
  }

  auto GetSizeEndpointHandler() -> cppcoro::task<int> {
    return entity_.GetSize();
  }

  Entity entity_{};
  vh::network::RestServer rest_server_;
};

class EntityClient : public EntityInterface {
 public:
  explicit EntityClient(vh::network::Uri base_uri)
      : client_{std::move(base_uri),
                test::restsdk::Injector()
                    .create<vh::cpp::NnUp<vh::network::IRestRequestSender>>()} {
  }

  auto PushSymbol(vh::stk::core::Symbol symbol) -> cppcoro::task<> override {
    co_return co_await client_.Call(EntityServer::PushSymbolEndpointDesc())
        .WithBody(symbol)
        .DiscardingResult();
  }

  auto GetSymbol [[nodiscard]] (int index) const
      -> cppcoro::task<vh::stk::core::Symbol> override {
    co_return co_await client_.Call(EntityServer::GetSymbolEndpointDesc())
        .WithParam("index", index)
        .AndReceive<vh::stk::core::Symbol>();
  }

  auto GetSize [[nodiscard]] () const -> cppcoro::task<int> override {
    co_return co_await client_.Call(EntityServer::GetSizeEndpointDesc())
        .AndReceive<int>();
  }

 private:
  vh::network::RestClient client_;
};

TEST(ClientServer, ApiTest) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    auto entity = Entity{};
    co_await entity.PushSymbol({"ETHUSDT"});
    co_await entity.PushSymbol({"BTCUSDT"});

    auto entity_server = EntityServer{kBaseUri};

    auto entity_client = EntityClient{kBaseUri};
    co_await entity_client.PushSymbol({"ETHUSDT"});
    co_await entity_client.PushSymbol({"BTCUSDT"});

    EXPECT_EQ(co_await entity.GetSize(), co_await entity_client.GetSize());
    EXPECT_EQ(co_await entity.GetSymbol(0),
              co_await entity_client.GetSymbol(0));
    EXPECT_EQ(co_await entity.GetSymbol(1),
              co_await entity_client.GetSymbol(1));
  }());
}

TEST(ClientServerDeathTest, WrongClientTypes) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    auto entity_server = EntityServer{kBaseUri};
    auto rest_client = vh::network::RestClient{
        kBaseUri,
        test::restsdk::Injector()
            .create<vh::cpp::NnUp<vh::network::IRestRequestSender>>()};

    EXPECT_DEATH(
        co_await rest_client.Call(EntityServer::PushSymbolEndpointDesc())
            .WithBody(0)
            .DiscardingResult(),
        "");
    EXPECT_DEATH(
        co_await rest_client.Call(EntityServer::PushSymbolEndpointDesc())
            .DiscardingResult(),
        "");
    EXPECT_DEATH(
        co_await rest_client.Call(EntityServer::PushSymbolEndpointDesc())
            .WithParam("NOT_EXPECTED_PARAM", 1)
            .WithBody("BTC")
            .DiscardingResult(),
        "");
    co_await rest_client.Call(EntityServer::PushSymbolEndpointDesc())
        .WithBody("BTC")
        .DiscardingResult();

    auto endpoint_with_added_param = EntityServer::GetSymbolEndpointDesc();
    endpoint_with_added_param.expected_types.params.emplace(
        "NOT_SENT_PARAM", vh::network::ExpectedType<int>());
    EXPECT_DEATH(co_await rest_client.Call(endpoint_with_added_param)
                     .WithParam("index", 0)
                     .AndReceive<vh::stk::core::Symbol>(),
                 "");
    EXPECT_DEATH(co_await rest_client.Call(endpoint_with_added_param)
                     .WithParam("index", 0)
                     .WithParam("UNKNOWN_PARAM", 0)
                     .AndReceive<vh::stk::core::Symbol>(),
                 "");
  }());
}

class FunctionHandler : public vh::network::IRestRequestHandler {
 public:
  explicit FunctionHandler(
      fu2::unique_function<cppcoro::task<vh::network::RestResponse>(
          vh::network::RestRequest) const>
          handler)
      : handler_{std::move(handler)} {}

  auto HandleRequestAndGiveResponse
      [[nodiscard]] (vh::network::RestRequest request)
      -> cppcoro::task<vh::network::RestResponse> override {
    co_return co_await handler_(request);
  }

 private:
  fu2::unique_function<cppcoro::task<vh::network::RestResponse>(
      vh::network::RestRequest) const>
      handler_{};
};

TEST(ClientServer, WrongClientTypesReceived) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    auto entity_client = EntityClient{kBaseUri};

    auto handlers =
        absl::flat_hash_map<vh::network::Endpoint,
                            vh::cpp::NnUp<vh::network::IRestRequestHandler>>{};

    handlers.emplace(
        EntityServer::PushSymbolEndpointDesc().endpoint,
        vh::cpp::MakeNnUp<FunctionHandler>(
            [](const auto &) -> cppcoro::task<vh::network::RestResponse> {
              co_return vh::network::RestResponse{
                  .status = vh::network::Status::kOk,
                  .result = vh::network::ConvertToJson(0)};
            }));
    handlers.emplace(
        EntityServer::GetSymbolEndpointDesc().endpoint,
        vh::cpp::MakeNnUp<FunctionHandler>(
            [](const auto &) -> cppcoro::task<vh::network::RestResponse> {
              co_return vh::network::RestResponse{.status =
                                                      vh::network::Status::kOk};
            }));
    handlers.emplace(
        EntityServer::PushSymbolEndpointDesc().endpoint,
        vh::cpp::MakeNnUp<FunctionHandler>(
            [](const auto &) -> cppcoro::task<vh::network::RestResponse> {
              co_return vh::network::RestResponse{
                  .status = vh::network::Status::kOk,
                  .result = vh::network::ConvertToJson("NOT_INT")};
            }));

    const auto entity_server = [&handlers]() {
      auto entity_server =
          test::restsdk::Injector()
              .create<vh::cpp::NnUp<vh::network::IRestRequestReceiver>>();
      entity_server->Receive(
          kBaseUri, vh::cpp::MakeNnUp<vh::network::EndpointRequestDispatcher>(
                        std::move(handlers)));
      return entity_server;
    }();

    EXPECT_ANY_THROW(co_await entity_client.PushSymbol({"BTC"}));
    EXPECT_ANY_THROW(co_await entity_client.GetSymbol(0));
    EXPECT_ANY_THROW(co_await entity_client.GetSize());
  }());
}

TEST(ClientServerDeathTest, WrongServerTypes) {
  auto entity = []() {
    auto entity = Entity{};
    cppcoro::sync_wait(entity.PushSymbol({"ETH"}));
    return entity;
  }();
  auto entity_client = EntityClient{kBaseUri};
  auto rest_server =
      vh::network::RestServerBuilder{
          kBaseUri,
          test::restsdk::Injector()
              .create<vh::cpp::NnUp<vh::network::IRestRequestReceiver>>()}
          .Handling(EntityServer::PushSymbolEndpointDesc(),
                    [&entity](auto request) -> cppcoro::task<int> {
                      co_await entity.PushSymbol(request.Body());
                      co_return 55;
                    })
          .Handling(EntityServer::GetSymbolEndpointDesc(),
                    [&entity](auto request) -> cppcoro::task<> {
                      co_await entity.GetSymbol(request.Param("index"));
                    })
          .Handling(EntityServer::GetSizeEndpointDesc(),
                    []() -> cppcoro::task<std::string> { co_return "NOT_INT"; })
          .Start();

  // TODO(vh): EXPECT_DEATH doesn't work here and just blocks the app.
  // Each of the following lines should be uncommented one by one
  // and abort the program.

  // entity_client.PushSymbol("BTC");
  // std::ignore = entity_client.GetSymbol(0);
  // std::ignore = entity_client.GetSize();
}

TEST(ClientServer, HandlingException) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    auto entity_server = EntityServer{kBaseUri};
    auto entity_client = EntityClient{kBaseUri};

    const auto client_exception =
        co_await [&entity_client]() -> cppcoro::task<std::string> {
      try {
        co_await entity_client.GetSymbol(2);
      } catch (const std::exception &exception) {
        co_return std::string{exception.what()};
      }

      std::terminate();
    }();

    EXPECT_EQ(client_exception, kIndexOutOfBoundsMessage);
  }());
}

TEST(ClientServer, ServerReceivedWrongTypeException) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    auto entity_server = EntityServer{kBaseUri};
    auto sender = test::restsdk::Injector()
                      .create<vh::cpp::NnUp<vh::network::IRestRequestSender>>();

    auto request =
        vh::network::RestRequestBuilder{}
            .WithMethod(EntityServer::PushSymbolEndpointDesc().endpoint.method)
            .WithBaseUri(kBaseUri)
            .AppendUri(EntityServer::PushSymbolEndpointDesc().endpoint.uri)
            .WithBody(123)
            .Build();

    auto response =
        co_await sender->SendRequestAndGetResponse(std::move(request));

    EXPECT_EQ(response.status, vh::network::Status::kBadRequest);
    EXPECT_NO_THROW(std::ignore =
                        vh::network::ParseFromJson<vh::cpp::MessageException>(
                            *response.result));

    request =
        vh::network::RestRequestBuilder{}
            .WithMethod(EntityServer::PushSymbolEndpointDesc().endpoint.method)
            .WithBaseUri(kBaseUri)
            .AppendUri(EntityServer::PushSymbolEndpointDesc().endpoint.uri)
            .AddParam("UNKNOWN_PARAM", 0)
            .Build();

    response = co_await sender->SendRequestAndGetResponse(std::move(request));

    EXPECT_EQ(response.status, vh::network::Status::kBadRequest);
    EXPECT_NO_THROW(std::ignore =
                        vh::network::ParseFromJson<vh::cpp::MessageException>(
                            *response.result));
  }());
}

TEST(ClientServer, ClientReceivedWrongTypeException) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    class Handler : public vh::network::IRestRequestHandler {
     public:
      auto HandleRequestAndGiveResponse
          [[nodiscard]] (vh::network::RestRequest /*unused*/)
          -> cppcoro::task<vh::network::RestResponse> override {
        co_return vh::network::RestResponse{.status = vh::network::Status::kOk};
      }

     private:
      mutable Entity entity_{};
    };

    const auto handler = []() {
      auto handler =
          test::restsdk::Injector()
              .create<vh::cpp::NnUp<vh::network::IRestRequestReceiver>>();
      handler->Receive(kBaseUri, vh::cpp::MakeNnUp<Handler>());
      return handler;
    }();
    const auto entity_client = EntityClient{kBaseUri};

    EXPECT_ANY_THROW(co_await entity_client.GetSize());
  }());
}
}  // namespace