#include "app_dt_stec_app_endpoints.h"

#include <string>

#include "app_dt_types.h"
#include "cpp_optional.h"
#include "cpp_typed_struct.h"
#include "network_enums.h"
#include "network_typed_endpoint.h"

namespace stonks::app::dt::stec::endpoints {
auto GetAveragePrice() -> network::TypedEndpoint {
  return {.endpoint = {.method = network::Method::kGet,
                       .uri = {"/GetAveragePrice"}},
          .expected_types = {
              .params = {{"symbol", network::ExpectedType<Symbol>()}},
              .result = network::ExpectedType<Price>()}};
}
}  // namespace stonks::app::dt::stec::endpoints