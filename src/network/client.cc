#include "client.h"

#include <cpprest/json.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <optional>
#include <variant>

#include "json_conversions.h"
#include "rest_request.h"

namespace stonks {
namespace {
auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("Client");
  return *logger;
}
}  // namespace

class Client::Impl {
 public:
  explicit Impl(std::string_view base_uri) : base_uri_{base_uri} {}

  [[nodiscard]] auto Execute(const EndpointDesc &endpoint,
                             const std::map<std::string, json::Any> &params,
                             const json::Any &request_body) const -> std::any {
    auto request = rest::RestRequest{endpoint.method, base_uri_}.AppendUri(
        endpoint.relative_uri);

    for (const auto &param : params) {
      // TODO (vh): check params and maybe throw if missing?
      request.any_AddParameterValue(param.first, param.second);
    }

    if (endpoint.request_body.has_value()) {
      if (const auto no_mandatory_object = !request_body.value.has_value() &&
                                           (endpoint.request_body->optional ==
                                            json::OptionalFlag::kMandatory)) {
        // TODO(vh): maybe throw?
        Logger().error("Request misses mandatory object");
        return {};
      }

      auto object_json = std::optional<web::json::value>{std::nullopt};

      object_json = std::visit(
          [&value = request_body.value](
              const auto &variant) -> std::optional<web::json::value> {
            try {
              return variant.any_ConvertToJson(value);
            } catch (...) {
              return std::nullopt;
            }
          },
          request_body.converter);

      if (const auto not_converted = !object_json.has_value()) {
        Logger().error("Request has wrong object type");
        return {};
      }

      request.SetJson(*object_json);
    }

    auto result = std::any{};
    const auto result_json = request.SendAndGetResponse();

    if (endpoint.response_body.has_value()) {
      if (!result_json.has_value()) {
        if (const auto no_mandatory_result = endpoint.response_body->optional ==
                                             json::OptionalFlag::kMandatory) {
          // TODO(vh): maybe throw?
          Logger().error("Response misses mandatory result");
        }

        return {};
      }

      result = std::visit(
          [&result_json](const auto &variant) -> std::any {
            return variant.any_ParseFromJson(*result_json);
          },
          endpoint.response_body->converter);

      if (const auto not_converted = !result.has_value()) {
        Logger().error("Response has wrong result type");
        return {};
      }

      return result;
    }

    // TODO(vh): nothrow here but throw in errors
    return {};
  }

 private:
  const std::string base_uri_{};
};

Client::~Client() = default;

Client::Client(std::string_view base_uri)
    : impl_{std::make_unique<Impl>(base_uri)} {}

auto Client::Execute(const EndpointDesc &endpoint,
                     const std::map<std::string, json::Any> &params,
                     const json::Any &object) const -> std::any {
  return impl_->Execute(endpoint, params, object);
}
}  // namespace stonks