#include <gtest/gtest-death-test.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <any>
#include <cstdio>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>

#include "finance_types.h"
#include "network_auto_parsable_request.h"
#include "network_auto_parsable_request_handler.h"
#include "network_endpoint_request_dispatcher.h"
#include "network_enums.h"
#include "network_i_rest_request_receiver.h"
#include "network_i_rest_request_sender.h"
#include "network_json_basic_conversions.h"
#include "network_rest_client.h"
#include "network_rest_server.h"
#include "network_typed_endpoint.h"
#include "network_types.h"
#include "not_null.hpp"
#include "restsdk_factory.h"
#include "stonks_types.h"

namespace {
const auto kBaseUri = "http://localhost:6506/Entity";

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
  static constexpr auto kIndexOutOfBoundsMessage = "Index out of bounds";

  void PushSymbol(stonks::SymbolName symbol) override {
    symbols_.emplace_back(std::move(symbol));
  }

  [[nodiscard]] auto GetSymbol(int index) const -> stonks::SymbolName override {
    if (symbols_.size() > index) {
      return symbols_[index];
    }

    throw std::runtime_error{kIndexOutOfBoundsMessage};
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
                    cpp::assume_not_null(
                        std::make_unique<stonks::restsdk::Factory>())}
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
    return entity_.GetSymbol(request.Param("index"));
  }

  auto GetSizeEndpointHandler() -> int { return entity_.GetSize(); }

  Entity entity_{};
  cpp::not_null<std::unique_ptr<stonks::network::IRestRequestReceiver>>
      request_receiver_;
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

TEST(ClientServer, ApiTest) {
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

  // const auto entity_exception = [&entity]() {
  //   try {
  //     std::ignore = entity.GetSymbol(2);
  //   } catch (const std::exception &exception) {
  //     return std::string{exception.what()};
  //   }

  //   std::terminate();
  // }();
  // const auto client_exception = [&entity_client]() {
  //   try {
  //     std::ignore = entity_client.GetSymbol(2);
  //   } catch (const std::exception &exception) {
  //     return std::string{exception.what()};
  //   }

  //   std::terminate();
  // }();
  // EXPECT_EQ(entity_exception, Entity::kIndexOutOfBoundsMessage);
  // EXPECT_EQ(client_exception, Entity::kIndexOutOfBoundsMessage);
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

TEST(ClientServerDeathTest, WrongServerTypes) {
  auto entity = []() {
    auto entity = Entity{};
    entity.PushSymbol("ETH");
    return entity;
  }();
  auto entity_client = EntityClient{kBaseUri};
  auto rest_server =
      stonks::network::RestServer{
          cpp::assume_not_null(std::make_unique<stonks::restsdk::Factory>())}
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
                    [&entity]() { return "NOT_INT"; })
          .Start();

  // TODO(vh): EXPECT_DEATH doesn't work here and just blocks the app. Each of
  // the following lines should be uncommented one by one and abort the program.

  // entity_client.PushSymbol("BTC");
  // std::ignore = entity_client.GetSymbol(0);
  // std::ignore = entity_client.GetSize();
}

// TEST(ClientServerDeathTest, ExceptionTest) {
// auto entity_server = EntityServer{kBaseUri};
// auto rest_client = stonks::network::RestClient{
//     kBaseUri, stonks::restsdk::Factory{}.CreateRestRequestSender()};

// auto endpoint = EntityServer::GetSizeEndpointDesc();
// EXPECT_NO_THROW(rest_client.Call(endpoint).DiscardingResult());

// endpoint = EntityServer::GetSizeEndpointDesc();
// endpoint.endpoint.method = stonks::network::Method::kPost;
// EXPECT_ANY_THROW(rest_client.Call(endpoint).DiscardingResult());

// endpoint = EntityServer::GetSizeEndpointDesc();
// endpoint.endpoint.uri += "BAD";
// EXPECT_ANY_THROW(rest_client.Call(endpoint).DiscardingResult());

// endpoint = EntityServer::PushSymbolEndpointDesc();
// EXPECT_NO_THROW(rest_client.Call(endpoint)
//                     .WithBody(stonks::SymbolName{})
//                     .DiscardingResult());

// endpoint = EntityServer::PushSymbolEndpointDesc();
// EXPECT_DEATH(rest_client.Call(endpoint).DiscardingResult(), "");

// endpoint = EntityServer::PushSymbolEndpointDesc();
// EXPECT_DEATH(rest_client.Call(endpoint).WithBody(33).DiscardingResult(),
// "");
// }
}  // namespace