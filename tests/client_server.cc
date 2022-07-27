#include <gtest/gtest-death-test.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <any>
#include <cstdio>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
#include <thread>

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
      -> stonks::network::Endpoint {
    return stonks::network::Endpoint{.method = stonks::network::Method::kPost,
                                     .uri = "/PushSymbol"};
  }

  [[nodiscard]] static auto GetSymbolEndpointDesc()
      -> stonks::network::Endpoint {
    return stonks::network::Endpoint{.method = stonks::network::Method::kGet,
                                     .uri = "/GetSymbol"};
  }

  [[nodiscard]] static auto GetSizeEndpointDesc() -> stonks::network::Endpoint {
    return stonks::network::Endpoint{.method = stonks::network::Method::kGet,
                                     .uri = "/GetSize"};
  }

  explicit EntityServer(std::string_view base_uri)
      : request_receiver_{
            stonks::network::RestServer{
                cpp::assume_not_null(
                    std::make_unique<stonks::restsdk::Factory>())}
                .On(base_uri.data())
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
  explicit EntityClient(std::string_view base_uri)
      : client_{base_uri,
                stonks::restsdk::Factory{}.CreateRestRequestSender()} {}

  void PushSymbol(stonks::SymbolName symbol) override {
    client_.Call(EntityServer::PushSymbolEndpointDesc())
        .WithBody(symbol)
        .DiscardingResult();
  }

  [[nodiscard]] auto GetSymbol(int index) const -> stonks::SymbolName override {
    return client_.Call(EntityServer::GetSymbolEndpointDesc())
        .WithParam("index", index)
        .AndReceive<stonks::SymbolName>();
  }

  [[nodiscard]] auto GetSize() const -> int override {
    return client_.Call(EntityServer::GetSizeEndpointDesc()).AndReceive<int>();
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
  // EXPECT_ANY_THROW(static_cast<void>(entity.GetSymbol(2)));
  // EXPECT_ANY_THROW(static_cast<void>(entity_client.GetSymbol(2)));

  // try {
  //   static_cast<void>(entity.GetSymbol(2));
  // } catch (const std::exception& exception) {
  //   EXPECT_STRCASEEQ(exception.what(), Entity::kIndexOutOfBoundsMessage);
  // }

  // try {
  //   static_cast<void>(entity_client.GetSymbol(2));
  // } catch (const std::exception& exception) {
  //   EXPECT_STRCASEEQ(exception.what(), Entity::kIndexOutOfBoundsMessage);
  // }
}

// TEST(ClientServerDeathTest, ExceptionTest) {
//   auto entity_server = EntityServer{kBaseUri};

//   class BadRequestClient : public stonks::network::Client {
//    public:
//     explicit BadRequestClient() : stonks::network::Client{kBaseUri} {}
//     void SendBadRequests() {
//       auto endpoint = EntityServer::GetSizeEndpointDesc();
//       EXPECT_NO_THROW(Execute(endpoint));

//       endpoint = EntityServer::GetSizeEndpointDesc();
//       endpoint.method = web::http::methods::POST;
//       EXPECT_ANY_THROW(Execute(endpoint));

//       endpoint = EntityServer::GetSizeEndpointDesc();
//       endpoint.relative_uri += "BAD";
//       EXPECT_ANY_THROW(Execute(endpoint));

//       endpoint = EntityServer::PushSymbolEndpointDesc();
//       EXPECT_NO_THROW(Execute(endpoint, {.body =
//       stonks::finance::Symbol{}}));

//       endpoint = EntityServer::PushSymbolEndpointDesc();
//       EXPECT_DEATH(Execute(endpoint), "");

//       endpoint = EntityServer::PushSymbolEndpointDesc();
//       EXPECT_DEATH(Execute(endpoint, {.body = 33}), "");
//     }
//   } client;

//   client.SendBadRequests();
// }
}  // namespace