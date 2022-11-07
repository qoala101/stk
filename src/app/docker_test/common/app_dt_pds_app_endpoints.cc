#include "app_dt_pds_app_endpoints.h"

#include <string>

#include "app_dt_types.h"
#include "cpp_typed_struct.h"
#include "network_enums.h"
#include "network_typed_endpoint.h"

namespace stonks::app::dt::pds::endpoints {
auto RecordSymbolPrice() -> network::TypedEndpoint {
  return {
      .endpoint = {.method = network::Method::kPost,
                   .uri = {"/RecordSymbolPrice"}},
      .expected_types = {.body = network::ExpectedType<SymbolPriceRecord>()}};
}
}  // namespace stonks::app::dt::pds::endpoints