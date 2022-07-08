#include "restsdk_rest_request_sender.h"

#include <bits/exception.h>
#include <cpprest/asyncrt_utils.h>
#include <cpprest/base_uri.h>
#include <cpprest/http_client.h>
#include <cpprest/http_headers.h>
#include <cpprest/http_msg.h>
#include <cpprest/uri_builder.h>
#include <fmt/format.h>
#include <pplx/pplxtasks.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <magic_enum.hpp>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "network_enums.h"
#include "network_types.h"
#include "restsdk_utils.h"

namespace stonks::restsdk {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("Client");
  return *logger;
}
}  // namespace

RestRequestSender::RestRequestSender(network::Endpoint endpoint)
    : endpoint_{std::move(endpoint)} {}

auto RestRequestSender::SendRequestAndGetResponse(
    const network::RestRequestData &data) const -> network::Json {
  auto uri_builder = [this, &data]() {
    auto uri_builder = web::http::uri_builder{endpoint_.uri};

    for (const auto &[key, value] : data.params) {
      uri_builder.append_query(key, value);
    }

    return uri_builder;
  }();

  auto http_request = [this, &data]() {
    auto http_request = web::http::http_request{
        utils::MethodFromNetworkMethod(endpoint_.method)};

    for (const auto &[key, value] : data.params) {
      http_request.headers().add(std::string{key}, std::string{value});
    }

    if (data.body.has_value()) {
      // http_request.set_body(data.body);
    }

    return http_request;
  }();

  const auto full_uri = uri_builder.to_uri();

  Logger().info("Sending {} request to {}",
                magic_enum::enum_name(endpoint_.method), full_uri.to_string());

  auto http_client = web::http::client::http_client{full_uri};

  try {
    const auto response = http_client.request(http_request).get();
    return network::Json{};
    // return network::Json{response.extract_json().get()};
  } catch (const std::exception &exception) {
    Logger().error("Request failed: {}", exception.what());
    throw;
  }
}
}  // namespace stonks::restsdk