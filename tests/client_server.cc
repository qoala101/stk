#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <any>

#include "client.h"
#include "client_server_types.h"
#include "finance_types.h"
#include "json_conversions.h"
#include "server.h"

namespace {

class EntityInterface {
 public:
  virtual ~EntityInterface() = default;

  virtual void PushSymbol(stonks::finance::Symbol symbol) = 0;

  [[nodiscard]] virtual auto GetSymbol(int index) const
      -> std::optional<stonks::finance::Symbol> = 0;

  [[nodiscard]] virtual auto GetSize() const -> int = 0;
};

class Entity : public EntityInterface {
 public:
  void PushSymbol(stonks::finance::Symbol symbol) override {
    symbols_.emplace_back(std::move(symbol));
  }

  [[nodiscard]] auto GetSymbol(int index) const
      -> std::optional<stonks::finance::Symbol> override {
    if (symbols_.size() > index) {
      return symbols_[index];
    }

    return std::nullopt;
  }

  [[nodiscard]] auto GetSize() const -> int override { return symbols_.size(); }

 private:
  std::vector<stonks::finance::Symbol> symbols_{};
};

class EntityServer : public stonks::Server {
 public:
  explicit EntityServer(std::string_view base_uri)
      : stonks::Server{
            base_uri,
            {PushSymbolEndpointDesc(this), GetSymbolEndpointDesc(this),
             GetSizeEndpointDesc(this)}} {}

  [[nodiscard]] static auto PushSymbolEndpointDesc(
      EntityServer *server = nullptr) -> stonks::EndpointDesc {
    return stonks::EndpointDesc{
        .method = web::http::methods::POST,
        .relative_uri = "/PushSymbol",
        .object =
            stonks::json::AnyDesc{
                .converter =
                    stonks::json::JsonConverter<stonks::finance::Symbol>{},
                .optional = stonks::json::OptionalFlag::kMandatory},
        .handler_ =
            [server](
                const std::map<std::string, stonks::json::Any> & /*params*/,
                stonks::json::Any object) {
              return server->PushSymbol(std::move(object));
            }};
  }

  [[nodiscard]] static auto GetSymbolEndpointDesc(
      EntityServer *server = nullptr) -> stonks::EndpointDesc {
    return stonks::EndpointDesc{
        .method = web::http::methods::GET,
        .relative_uri = "/GetSymbol",
        .params = {{"index",
                    stonks::json::AnyDesc{
                        .converter = stonks::json::JsonConverter<int>{},
                        .optional = stonks::json::OptionalFlag::kMandatory}}},
        .result =
            stonks::json::AnyDesc{
                .converter =
                    stonks::json::JsonConverter<stonks::finance::Symbol>{},
                .optional = stonks::json::OptionalFlag::kOptional},
        .handler_ = [server](std::map<std::string, stonks::json::Any> params,
                             const stonks::json::Any & /*object*/) {
          return server->GetSymbol(std::move(params));
        }};
  }

  [[nodiscard]] static auto GetSizeEndpointDesc(EntityServer *server = nullptr)
      -> stonks::EndpointDesc {
    return stonks::EndpointDesc{
        .method = web::http::methods::GET,
        .relative_uri = "/GetSize",
        .result =
            stonks::json::AnyDesc{
                .converter = stonks::json::JsonConverter<int>{},
                .optional = stonks::json::OptionalFlag::kMandatory},
        .handler_ =
            [server](
                const std::map<std::string, stonks::json::Any> & /*params*/,
                const stonks::json::Any & /*object*/) {
              return server->GetSize();
            }};
  }

 private:
  [[nodiscard]] auto PushSymbol(stonks::json::Any object) -> std::any {
    spdlog::info("PushSymbol!!");
    entity_.PushSymbol(
        // TODO(vh): if object is optional, cast should be to std::optional and
        // function should take optional object
        std::any_cast<stonks::finance::Symbol>(std::move(object.value)));
    return {};
  }

  [[nodiscard]] auto GetSymbol(
      std::map<std::string, stonks::json::Any> params) const -> std::any {
    spdlog::info("GetSymbol!!");
    return *entity_.GetSymbol(
        // TODO(vh): if param is optional, cast should be to std::optional and
        // function should take optional param
        std::any_cast<int>(std::move(params.at("index").value)));
  }

  [[nodiscard]] auto GetSize() const -> std::any {
    spdlog::info("GetSize!!");
    return entity_.GetSize();
  }

  Entity entity_{};
};

class EntityClient : public stonks::Client, public EntityInterface {
 public:
  explicit EntityClient(std::string_view base_uri) : stonks::Client{base_uri} {}

  void PushSymbol(stonks::finance::Symbol symbol) override {
    Execute(EntityServer::PushSymbolEndpointDesc(), {},
            stonks::json::Any{
                .value = symbol,
                .converter =
                    stonks::json::JsonConverter<stonks::finance::Symbol>{}});
  }

  [[nodiscard]] auto GetSymbol(int index) const
      -> std::optional<stonks::finance::Symbol> override {
    return std::any_cast<stonks::finance::Symbol>(
        Execute(EntityServer::GetSymbolEndpointDesc(),
                {{{"index",
                   stonks::json::Any{
                       .value = index,
                       .converter = stonks::json::JsonConverter<int>{}}}}},
                stonks::json::Any{}));
  }

  [[nodiscard]] auto GetSize() const -> int override {
    return std::any_cast<int>(
        Execute(EntityServer::GetSizeEndpointDesc(), {}, {}));
  }
};

TEST(ClientServer, Test1) {
  auto entity = Entity{};
  entity.PushSymbol(
      stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"});
  entity.PushSymbol(
      stonks::finance::Symbol{.base_asset = "BTC", .quote_asset = "USDT"});

  const auto base_uri = "http://localhost:6506/Entity";
  auto entity_server = EntityServer{base_uri};

  auto entity_client = EntityClient{base_uri};
  entity_client.PushSymbol(
      stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"});
  entity_client.PushSymbol(
      stonks::finance::Symbol{.base_asset = "BTC", .quote_asset = "USDT"});

  EXPECT_EQ(entity.GetSize(), entity_client.GetSize());
  EXPECT_EQ(entity.GetSymbol(0), entity_client.GetSymbol(0));
  EXPECT_EQ(entity.GetSymbol(1), entity_client.GetSymbol(1));
}
}  // namespace