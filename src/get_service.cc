#include "get_service.h"

using web::uri;
using web::uri_builder;
using web::http::http_request;
using web::http::methods;
using web::http::status_codes;
using web::http::experimental::listener::http_listener;
namespace json = web::json;

namespace stonks
{
  void GetService::handle_get(http_request message)
  {
    const auto relative_uri = uri::decode(message.relative_uri().path());
    const auto uri_parts = uri::split_path(relative_uri);

    std::cout << "stonks: GET on ";

    for (const auto &uri_part : uri_parts)
    {
      std::cout << "/" << uri_part;
    }

    std::cout << std::endl;

    if (uri_parts.empty())
    {
      message.reply(status_codes::NotFound);
      return;
    }

    auto response = json::value::object();
    response["app"] = json::value::string("stonks");
    response["version"] = json::value::string("0.0.1");
    message.reply(status_codes::OK, response);
  }

  pplx::task<void> GetService::run()
  {
    auto endpoint_builder = uri_builder{};
    endpoint_builder.set_scheme("http");
    endpoint_builder.set_host("localhost");
    endpoint_builder.set_port(6502);
    endpoint_builder.set_path("stonks");

    http_listener_ = http_listener(endpoint_builder.to_uri());
    http_listener_.support(methods::GET, std::bind(&GetService::handle_get, this, std::placeholders::_1));
    return http_listener_.open();
  }

  pplx::task<void> GetService::stop()
  {
    return http_listener_.close();
  }

  std::string GetService::endpoint() const
  {
    return http_listener_.uri().to_string();
  }
}