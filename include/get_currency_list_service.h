#ifndef STONKS_GET_CURRENCY_LIST_SERVICE_H_
#define STONKS_GET_CURRENCY_LIST_SERVICE_H_

#include <cpprest/http_listener.h>
#include <pplx/pplxtasks.h>

namespace stonks
{
  class GetCurrencyListService
  {
  public:
    pplx::task<void> Start();
    pplx::task<void> Stop();

  private:
    web::http::experimental::listener::http_listener http_listener_;
  };
}

#endif // STONKS_GET_CURRENCY_LIST_SERVICE_H_
