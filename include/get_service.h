#ifndef STONKS_MICROSVC_CONTROLLER
#define STONKS_MICROSVC_CONTROLLER

#include <string>

#include <cpprest/http_listener.h>
#include <cpprest/http_msg.h>
#include <pplx/pplxtasks.h>

namespace stonks
{
  class GetService
  {
  public:
    void handle_get(web::http::http_request message);
    void handle_post(web::http::http_request message);

    pplx::task<void> run();
    pplx::task<void> stop();

    std::string endpoint() const;

  private:
    web::http::experimental::listener::http_listener http_listener_;
  };
}

#endif // STONKS_MICROSVC_CONTROLLER