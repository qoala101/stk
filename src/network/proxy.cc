#include "proxy.h"

#include <bits/exception.h>
#include <cpprest/base_uri.h>
#include <cpprest/http_headers.h>
#include <fmt/format.h>
#include <pplx/pplxtasks.h>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <ext/alloc_traits.h>
#include <functional>
#include <gsl/assert>
#include <memory>
#include <utility>
#include <vector>

#include "cpprest/http_msg.h"
#include "pplx/pplx.h"
#include "uri.h"

namespace stonks::network {
namespace {
auto Logger() -> spdlog::logger & {
  static auto logger = []() {
    auto logger = spdlog::stdout_color_mt("Proxy");
    logger->set_level(spdlog::level::debug);
    return logger;
  }();

  return *logger;
}

constexpr auto kFirstAvailablePort = 49152;
}  // namespace

Proxy::Proxy()
    : http_listener_{LocalUri{kPort, ""}.GetFullUri()},
      last_available_port_{kFirstAvailablePort} {
  http_listener_.support(std::bind_front(&Proxy::HandleRequest, this));
  http_listener_.open();
  Logger().info("Started proxy on {}", http_listener_.uri().path());
}

Proxy::~Proxy() {
  Logger().info("Shutting down proxy on {}", http_listener_.uri().path());
  http_listener_.close().wait();
}

auto Proxy::GetEndpointPort(std::string_view endpoint) const
    -> std::optional<int> {
  {
    const auto reader_lock = std::shared_lock{mutex_};

    const auto registered_endpoint = endpoints_.find(std::string{endpoint});

    if (const auto endpoint_not_registered =
            registered_endpoint == endpoints_.end()) {
      return std::nullopt;
    }

    return registered_endpoint->second.base_uri().port();
  }
}

void Proxy::RegisterEndpoint(std::string_view endpoint) {
  Expects(!endpoint.empty());
  Expects(endpoint.starts_with("/"));

  // TODO(vh): check for availability of ports maybe using boost
  auto available_port = 0;

  {
    const auto writer_lock = std::unique_lock{mutex_};

    available_port = last_available_port_++;
    const auto endpoint_uri =
        "http://localhost:" + std::to_string(available_port);
    endpoints_.emplace(endpoint, endpoint_uri);
  }

  Logger().debug("Registered {} on port {}", endpoint, available_port);
}

void Proxy::HandleRequest(const web::http::http_request &request) {
  const auto path_parts = web::uri::split_path(request.request_uri().path());

  if (path_parts.empty()) {
    request.reply(web::http::status_codes::NotFound);
    return;
  }

  const auto slash_endpoint = "/" + path_parts[0];
  auto response_task = pplx::task<web::http::http_response>{};

  {
    const auto reader_lock = std::shared_lock{mutex_};

    const auto endpoint = endpoints_.find(slash_endpoint);

    if (const auto endpoint_not_registered = endpoint == endpoints_.end()) {
      Logger().debug("Got request on not registered endpoint {}",
                     slash_endpoint);
      request.reply(web::http::status_codes::NotFound);
      return;
    }

    response_task = endpoint->second.request(request);
  }

  response_task
      .then([&request](web::http::http_response response) {
        response.headers().add("Access-Control-Allow-Origin", "*");
        return request.reply(response);
      })
      .wait();
}
}  // namespace stonks::network