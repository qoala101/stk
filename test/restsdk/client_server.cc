#include <gtest/gtest-death-test.h>
#include <gtest/gtest.h>

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "cpp_not_null.h"
#include "cpp_polymorphic_value.h"
#include "gtest/gtest_pred_impl.h"
#include "network_auto_parsable.h"
#include "network_auto_parsable_request.h"
#include "network_endpoint_request_dispatcher.h"
#include "network_enums.h"
#include "network_i_factory.h"
#include "network_i_rest_request_handler.h"
#include "network_i_rest_request_receiver.h"
#include "network_i_rest_request_sender.h"
#include "network_json_basic_conversions.h"
#include "network_rest_client.h"
#include "network_rest_client_request_builder.h"
#include "network_rest_request_builder.h"
#include "network_rest_server.h"
#include "network_typed_endpoint.h"
#include "network_types.h"
#include "restsdk_factory.h"
#include "stonks_types.h"

namespace {
const auto kBaseUri = "http://localhost:6506/Entity";
const auto kIndexOutOfBoundsMessage = "Index out of bounds";

class EntityInterface {
 public:
  virtual ~EntityInterface() = default;

  virtual void PushSymbol(stonks::SymbolName symbol) = 0;

  [[nodiscard]] virtual auto GetSymbol(int index) const
      -> stonks::SymbolName = 0;

  [[nodiscard]] virtual auto GetSize() const -> int = 0;
};

class Entity : public EntityInterface {
 public:
  void PushSymbol(stonks::SymbolName symbol) override {
    symbols_.emplace_back(std::move(symbol));
  }

  [[nodiscard]] auto GetSymbol(int index) const -> stonks::SymbolName override {
    assert(symbols_.size() > index);
    return symbols_[index];
  }

  [[nodiscard]] auto GetSize() const -> int override { return symbols_.size(); }

 private:
  std::vector<stonks::SymbolName> symbols_{};
};

class EntityServer {
 public:
  [[nodiscard]] static auto PushSymbolEndpointDesc()
      -> stonks::network::TypedEndpoint {
    return stonks::network::TypedEndpoint{
        .endpoint = {.method = stonks::network::Method::kPost,
                     .uri = "/PushSymbol"},
        .expected_types = {
            .body = stonks::network::ExpectedType<stonks::SymbolName>()}};
  }

  [[nodiscard]] static auto GetSymbolEndpointDesc()
      -> stonks::network::TypedEndpoint {
    return stonks::network::TypedEndpoint{
        .endpoint = {.method = stonks::network::Method::kGet,
                     .uri = "/GetSymbol"},
        .expected_types = {
            .params = {{"index", stonks::network::ExpectedType<int>()}},
            .result = stonks::network::ExpectedType<stonks::SymbolName>()}};
  }

  [[nodiscard]] static auto GetSizeEndpointDesc()
      -> stonks::network::TypedEndpoint {
    return stonks::network::TypedEndpoint{
        .endpoint = {.method = stonks::network::Method::kGet,
                     .uri = "/GetSize"},
        .expected_types = {.result = stonks::network::ExpectedType<int>()}};
  }

  explicit EntityServer(std::string_view base_uri)
      : request_receiver_{
            std::move(
                stonks::network::RestServer{
                    stonks::cpp::MakeNnUp<stonks::restsdk::Factory>()}
                    .On(base_uri.data())
                    .Handling(
                        PushSymbolEndpointDesc(),
                        std::bind_front(
                            &EntityServer::PushSymbolEndpointHandler, this))
                    .Handling(
                        GetSymbolEndpointDesc(),
                        std::bind_front(&EntityServer::GetSymbolEndpointHandler,
                                        this))
                    .Handling(GetSizeEndpointDesc(),
                              std::bind_front(
                                  &EntityServer::GetSizeEndpointHandler, this)))
                .Start()} {}

 private:
  void PushSymbolEndpointHandler(
      stonks::network::AutoParsableRestRequest request) {
    entity_.PushSymbol(request.Body());
  }

  auto GetSymbolEndpointHandler(
      stonks::network::AutoParsableRestRequest request) -> stonks::SymbolName {
    const auto index = int{request.Param("index")};

    if (index >= entity_.GetSize()) {
      throw std::runtime_error{kIndexOutOfBoundsMessage};
    }

    return entity_.GetSymbol(index);
  }

  auto GetSizeEndpointHandler() -> int { return entity_.GetSize(); }

  Entity entity_{};
  stonks::cpp::NnUp<stonks::network::IRestRequestReceiver> request_receiver_;
};

class EntityClient : public EntityInterface {
 public:
  explicit EntityClient(std::string base_uri)
      : client_{std::move(base_uri),
                stonks::restsdk::Factory{}.CreateRestRequestSender()} {}

  void PushSymbol(stonks::SymbolName symbol) override {
    std::move(
        client_.Call(EntityServer::PushSymbolEndpointDesc()).WithBody(symbol))
        .DiscardingResult();
  }

  [[nodiscard]] auto GetSymbol(int index) const -> stonks::SymbolName override {
    return std::move(client_.Call(EntityServer::GetSymbolEndpointDesc())
                         .WithParam("index", index))
        .AndReceive<stonks::SymbolName>();
  }

  [[nodiscard]] auto GetSize() const -> int override {
    return std::move(client_.Call(EntityServer::GetSizeEndpointDesc()))
        .AndReceive<int>();
  }

 private:
  stonks::network::RestClient client_;
};

TEST(ClientServerDeathTest, ApiTest) {
  auto entity = Entity{};
  entity.PushSymbol("ETHUSDT");
  entity.PushSymbol("BTCUSDT");

  auto entity_server = EntityServer{kBaseUri};

  auto entity_client = EntityClient{kBaseUri};
  entity_client.PushSymbol("ETHUSDT");
  entity_client.PushSymbol("BTCUSDT");

  EXPECT_EQ(entity.GetSize(), entity_client.GetSize());
  EXPECT_EQ(entity.GetSymbol(0), entity_client.GetSymbol(0));
  EXPECT_EQ(entity.GetSymbol(1), entity_client.GetSymbol(1));
}

TEST(ClientServerDeathTest, WrongClientTypes) {
  auto entity_server = EntityServer{kBaseUri};
  auto rest_client = stonks::network::RestClient{
      kBaseUri, stonks::restsdk::Factory{}.CreateRestRequestSender()};

  EXPECT_DEATH(rest_client.Call(EntityServer::PushSymbolEndpointDesc())
                   .WithBody(0)
                   .DiscardingResult(),
               "");
  EXPECT_DEATH(rest_client.Call(EntityServer::PushSymbolEndpointDesc())
                   .DiscardingResult(),
               "");
  EXPECT_DEATH(rest_client.Call(EntityServer::PushSymbolEndpointDesc())
                   .WithParam("NOT_EXPECTED_PARAM", 1)
                   .WithBody("BTC")
                   .DiscardingResult(),
               "");
  rest_client.Call(EntityServer::PushSymbolEndpointDesc())
      .WithBody("BTC")
      .DiscardingResult();

  auto endpoint_with_added_param = EntityServer::GetSymbolEndpointDesc();
  endpoint_with_added_param.expected_types.params["NOT_SENT_PARAM"] =
      stonks::network::ExpectedType<int>();
  EXPECT_DEATH(std::ignore = rest_client.Call(endpoint_with_added_param)
                                 .WithParam("index", 0)
                                 .AndReceive<stonks::SymbolName>(),
               "");
  EXPECT_DEATH(std::ignore = rest_client.Call(endpoint_with_added_param)
                                 .WithParam("index", 0)
                                 .WithParam("UNKNOWN_PARAM", 0)
                                 .AndReceive<stonks::SymbolName>(),
               "");
}

class FunctionHandler : public stonks::network::IRestRequestHandler {
 public:
  explicit FunctionHandler(
      std::function<stonks::network::RestResponse(stonks::network::RestRequest)>
          handler)
      : handler_{std::move(handler)} {}

  [[nodiscard]] auto HandleRequestAndGiveResponse(
      stonks::network::RestRequest request) const
      -> stonks::network::RestResponse override {
    return handler_(request);
  }

 private:
  std::function<stonks::network::RestResponse(stonks::network::RestRequest)>
      handler_{};
};

TEST(ClientServerDeathTest, WrongClientTypesReceived) {
  auto entity_client = EntityClient{kBaseUri};

  auto handlers =
      std::map<stonks::network::Endpoint,
               stonks::cpp::NnUp<stonks::network::IRestRequestHandler>>{};

  handlers.emplace(EntityServer::PushSymbolEndpointDesc().endpoint,
                   stonks::cpp::MakeNnUp<FunctionHandler>(
                       [](const stonks::network::RestRequest& /*unused*/) {
                         return stonks::network::RestResponse{
                             .status = stonks::network::Status::kOk,
                             .result = stonks::network::ConvertToJson(0)};
                       }));
  handlers.emplace(EntityServer::GetSymbolEndpointDesc().endpoint,
                   stonks::cpp::MakeNnUp<FunctionHandler>(
                       [](const stonks::network::RestRequest& /*unused*/) {
                         return stonks::network::RestResponse{
                             .status = stonks::network::Status::kOk};
                       }));
  handlers.emplace(
      EntityServer::PushSymbolEndpointDesc().endpoint,
      stonks::cpp::MakeNnUp<FunctionHandler>(
          [](const stonks::network::RestRequest& /*unused*/) {
            return stonks::network::RestResponse{
                .status = stonks::network::Status::kOk,
                .result = stonks::network::ConvertToJson("NOT_INT")};
          }));

  auto entity_server = stonks::restsdk::Factory{}.CreateRestRequestReceiver(
      kBaseUri,
      stonks::cpp::MakeNnUp<stonks::network::EndpointRequestDispatcher>(
          std::move(handlers)));

  EXPECT_ANY_THROW(entity_client.PushSymbol("BTC"));
  EXPECT_ANY_THROW(std::ignore = entity_client.GetSymbol(0));
  EXPECT_ANY_THROW(std::ignore = entity_client.GetSize());
}

TEST(ClientServerDeathTest, WrongServerTypes) {
  auto entity = []() {
    auto entity = Entity{};
    entity.PushSymbol("ETH");
    return entity;
  }();
  auto entity_client = EntityClient{kBaseUri};
  auto rest_server =
      std::move(
          stonks::network::RestServer{
              stonks::cpp::MakeNnUp<stonks::restsdk::Factory>()}
              .On(kBaseUri)
              .Handling(
                  EntityServer::PushSymbolEndpointDesc(),
                  [&entity](stonks::network::AutoParsableRestRequest request) {
                    entity.PushSymbol(request.Body());
                    return 55;
                  })
              .Handling(
                  EntityServer::GetSymbolEndpointDesc(),
                  [&entity](stonks::network::AutoParsableRestRequest request) {
                    std::ignore = entity.GetSymbol(request.Param("index"));
                  })
              .Handling(EntityServer::GetSizeEndpointDesc(),
                        [&entity]() { return "NOT_INT"; }))
          .Start();

  // TODO(vh): EXPECT_DEATH doesn't work here and just blocks the app.
  // Each of the following lines should be uncommented one by one
  // and abort the program.

  // entity_client.PushSymbol("BTC");
  // std::ignore = entity_client.GetSymbol(0);
  // std::ignore = entity_client.GetSize();
}

TEST(ClientServerDeathTest, HandlingException) {
  auto entity_server = EntityServer{kBaseUri};
  auto entity_client = EntityClient{kBaseUri};

  const auto client_exception = [&entity_client]() {
    try {
      std::ignore = entity_client.GetSymbol(2);
    } catch (const std::exception& exception) {
      return std::string{exception.what()};
    }

    std::terminate();
  }();

  EXPECT_EQ(client_exception, kIndexOutOfBoundsMessage);
}

TEST(ClientServer, ServerReceivedWrongTypeException) {
  auto entity_server = EntityServer{kBaseUri};
  auto sender = stonks::restsdk::Factory{}.CreateRestRequestSender();

  auto request =
      std::move(
          stonks::network::RestRequestBuilder{}
              .WithMethod(
                  EntityServer::PushSymbolEndpointDesc().endpoint.method)
              .WithBaseUri(kBaseUri)
              .AppendUri(EntityServer::PushSymbolEndpointDesc().endpoint.uri)
              .WithBody(stonks::network::ConvertToJson(123)))
          .Build();

  auto response = sender->SendRequestAndGetResponse(std::move(request));

  EXPECT_EQ(response.status, stonks::network::Status::kBadRequest);
  ASSERT_TRUE(response.result.has_value());
  EXPECT_NO_THROW(
      std::ignore =
          stonks::network::ParseFromJson<stonks::cpp::MessageException>(
              **response.result));

  request =
      std::move(
          stonks::network::RestRequestBuilder{}
              .WithMethod(
                  EntityServer::PushSymbolEndpointDesc().endpoint.method)
              .WithBaseUri(kBaseUri)
              .AppendUri(EntityServer::PushSymbolEndpointDesc().endpoint.uri)
              .AddParam("UNKNOWN_PARAM", 0))
          .Build();

  response = sender->SendRequestAndGetResponse(std::move(request));

  EXPECT_EQ(response.status, stonks::network::Status::kBadRequest);
  ASSERT_TRUE(response.result.has_value());
  EXPECT_NO_THROW(
      std::ignore =
          stonks::network::ParseFromJson<stonks::cpp::MessageException>(
              **response.result));
}

TEST(ClientServer, ClientReceivedWrongTypeException) {
  class Handler : public stonks::network::IRestRequestHandler {
   public:
    [[nodiscard]] auto HandleRequestAndGiveResponse(
        stonks::network::RestRequest request) const
        -> stonks::network::RestResponse override {
      return {.status = stonks::network::Status::kOk};
    }

   private:
    mutable Entity entity_{};
  };

  auto handler = stonks::restsdk::Factory{}.CreateRestRequestReceiver(
      kBaseUri, stonks::cpp::MakeNnUp<Handler>());
  auto entity_client = EntityClient{kBaseUri};

  EXPECT_ANY_THROW(std::ignore = entity_client.GetSize());
}
}  // namespace