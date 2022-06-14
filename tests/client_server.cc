#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <any>
#include <cstdio>
#include <exception>
#include <functional>
#include <stdexcept>
#include <thread>

#include "client.h"
#include "endpoint.h"
#include "finance_types.h"
#include "server.h"

#include "uri.h"

namespace {
const auto kBaseUri = stonks::network::LocalUri{6506, "/Entity"};

class EntityInterface {
 public:
  virtual ~EntityInterface() = default;

  virtual void PushSymbol(stonks::finance::Symbol symbol) = 0;

  [[nodiscard]] virtual auto GetSymbol(int index) const
      -> stonks::finance::Symbol = 0;

  [[nodiscard]] virtual auto GetSize() const -> int = 0;
};

class Entity : public EntityInterface {
 public:
  static constexpr auto kIndexOutOfBoundsMessage = "Index out of bounds";

  void PushSymbol(stonks::finance::Symbol symbol) override {
    symbols_.emplace_back(std::move(symbol));
  }

  [[nodiscard]] auto GetSymbol(int index) const
      -> stonks::finance::Symbol override {
    if (symbols_.size() > index) {
      return symbols_[index];
    }

    throw std::runtime_error{kIndexOutOfBoundsMessage};
  }

  [[nodiscard]] auto GetSize() const -> int override { return symbols_.size(); }

 private:
  std::vector<stonks::finance::Symbol> symbols_{};
};

namespace {}  // namespace
class EntityServer : public stonks::network::Server {
 public:
  [[nodiscard]] static auto PushSymbolEndpointDesc()
      -> stonks::network::EndpointDesc {
    return stonks::network::EndpointDesc{.method = web::http::methods::POST,
                                         .relative_uri = "/PushSymbol"};
  }

  [[nodiscard]] static auto GetSymbolEndpointDesc()
      -> stonks::network::EndpointDesc {
    return stonks::network::EndpointDesc{.method = web::http::methods::GET,
                                         .relative_uri = "/GetSymbol"};
  }

  [[nodiscard]] static auto GetSizeEndpointDesc()
      -> stonks::network::EndpointDesc {
    return stonks::network::EndpointDesc{.method = web::http::methods::GET,
                                         .relative_uri = "/GetSize"};
  }

  explicit EntityServer(const stonks::network::LocalUri& base_uri)
      : stonks::network::Server{
            base_uri,
            {stonks::network::Endpoint{.desc = PushSymbolEndpointDesc(),
                                       .handler = PushSymbolEndpointHandler()},
             stonks::network::Endpoint{.desc = GetSymbolEndpointDesc(),
                                       .handler = GetSymbolEndpointHandler()},
             stonks::network::Endpoint{.desc = GetSizeEndpointDesc(),
                                       .handler = GetSizeEndpointHandler()}}} {}

 private:
  [[nodiscard]] auto PushSymbolEndpointHandler()
      -> stonks::network::NoResultTakesBody {
    return [this](stonks::network::Body request_body) {
      entity_.PushSymbol(request_body.Parse<stonks::finance::Symbol>());
    };
  }

  [[nodiscard]] auto GetSymbolEndpointHandler()
      -> stonks::network::HasResultTakesParams {
    return [this](stonks::network::Params params) {
      return entity_.GetSymbol(params["index"].Parse<int>());
    };
  }

  [[nodiscard]] auto GetSizeEndpointHandler() -> stonks::network::HasResult {
    return [this]() { return entity_.GetSize(); };
  }

  Entity entity_{};
};

class EntityClient : public stonks::network::Client, public EntityInterface {
 public:
  explicit EntityClient(const stonks::network::Uri& base_uri)
      : stonks::network::Client{base_uri} {}

  void PushSymbol(stonks::finance::Symbol symbol) override {
    Execute(EntityServer::PushSymbolEndpointDesc(), {.body = symbol});
  }

  [[nodiscard]] auto GetSymbol(int index) const
      -> stonks::finance::Symbol override {
    return std::any_cast<stonks::finance::Symbol>(Execute(
        EntityServer::GetSymbolEndpointDesc(), {.params = {{"index", index}}}));
  }

  [[nodiscard]] auto GetSize() const -> int override {
    return std::any_cast<int>(Execute(EntityServer::GetSizeEndpointDesc()));
  }
};

TEST(ClientServer, ApiTest) {
  auto entity = Entity{};
  entity.PushSymbol(
      stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"});
  entity.PushSymbol(
      stonks::finance::Symbol{.base_asset = "BTC", .quote_asset = "USDT"});

  auto entity_server = EntityServer{kBaseUri};

  auto entity_client = EntityClient{kBaseUri};
  entity_client.PushSymbol(
      stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"});
  entity_client.PushSymbol(
      stonks::finance::Symbol{.base_asset = "BTC", .quote_asset = "USDT"});

  EXPECT_EQ(entity.GetSize(), entity_client.GetSize());
  EXPECT_EQ(entity.GetSymbol(0), entity_client.GetSymbol(0));
  EXPECT_EQ(entity.GetSymbol(1), entity_client.GetSymbol(1));
  EXPECT_ANY_THROW(static_cast<void>(entity.GetSymbol(2)));
  EXPECT_ANY_THROW(static_cast<void>(entity_client.GetSymbol(2)));

  try {
    static_cast<void>(entity.GetSymbol(2));
  } catch (const std::exception& exception) {
    EXPECT_STRCASEEQ(exception.what(), Entity::kIndexOutOfBoundsMessage);
  }

  try {
    static_cast<void>(entity_client.GetSymbol(2));
  } catch (const std::exception& exception) {
    EXPECT_STRCASEEQ(exception.what(), Entity::kIndexOutOfBoundsMessage);
  }
}

TEST(ClientServerDeathTest, ExceptionTest) {
  auto entity_server = EntityServer{kBaseUri};

  class BadRequestClient : public stonks::network::Client {
   public:
    explicit BadRequestClient() : stonks::network::Client{kBaseUri} {}
    void SendBadRequests() {
      auto endpoint = EntityServer::GetSizeEndpointDesc();
      EXPECT_NO_THROW(Execute(endpoint));

      endpoint = EntityServer::GetSizeEndpointDesc();
      endpoint.method = web::http::methods::POST;
      EXPECT_ANY_THROW(Execute(endpoint));

      endpoint = EntityServer::GetSizeEndpointDesc();
      endpoint.relative_uri += "BAD";
      EXPECT_ANY_THROW(Execute(endpoint));

      endpoint = EntityServer::PushSymbolEndpointDesc();
      EXPECT_NO_THROW(Execute(endpoint, {.body = stonks::finance::Symbol{}}));

      endpoint = EntityServer::PushSymbolEndpointDesc();
      EXPECT_DEATH(Execute(endpoint), "");

      endpoint = EntityServer::PushSymbolEndpointDesc();
      EXPECT_DEATH(Execute(endpoint, {.body = 33}), "");
    }
  } client;

  client.SendBadRequests();
}
}  // namespace