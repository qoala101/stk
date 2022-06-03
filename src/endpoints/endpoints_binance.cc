#include "endpoints_binance.h"

#include <cpprest/http_msg.h>

#include <optional>
#include <string>

#include "binance_types.h"
#include "type.h"
#include "type_variant.h"

namespace stonks {
auto endpoints::Binance::GetExchangeInfo() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/api/v3/exchangeInfo",
          .response_body = json::Type<binance::ExchangeInfo>{}};
}
}  // namespace stonks