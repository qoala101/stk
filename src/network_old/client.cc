#include "client.h"

#include <absl/base/macros.h>
#include <bits/exception.h>
#include <cpprest/details/basic_types.h>
#include <cpprest/json.h>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "endpoint.h"
#include "rest_request.h"



namespace stonks::network {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("Client");
  return *logger;
}

void AddParamsToRequest(const Params &params, RestRequest &request) {
  for (const auto &[param_name, param_value] : params) {
    request.AddParameter(param_name, param_value.GetJson().serialize());
  }
}

void AddBodyToRequest(const Body &request_body, RestRequest &request) {
  const auto &request_json = request_body.GetJson();

  if (!request_json.is_null()) {
    request.SetJson(request_json);
  }
}

auto SendRequestAndGetResponse(const EndpointDesc &endpoint,
                               RestRequest &request) -> Result {
  Logger().info("Sending {} request to {}", endpoint.method,
                endpoint.relative_uri);

  auto response_json = request.SendAndGetResponse();

  if (const auto request_failed = !response_json.has_value()) {
    throw std::runtime_error{"Request failed"};
  }

  return Result{*std::move(response_json)};
}
}  // namespace

Client::Client(const Uri &uri) : base_uri_{uri.GetFullUri()} {}

auto Client::Execute(const EndpointDesc &endpoint,
                     const ExecuteArgs &args) const -> Result {
  auto request =
      RestRequest{endpoint.method, base_uri_}.AppendUri(endpoint.relative_uri);

  AddParamsToRequest(args.params, request);
  AddBodyToRequest(args.body, request);
  return SendRequestAndGetResponse(endpoint, request);
}
}  // namespace stonks::network