#include "endpoints_v1_strategy.h"

#include <cpprest/http_msg.h>

#include <optional>
#include <string>
#include <vector>

#include "finance_types.h"
#include "type.h"
#include "type_variant.h"

namespace stonks {
auto endpoints::V1Strategy::Run() -> network::EndpointDesc {
  return {.method = web::http::methods::GET, .relative_uri = "/Run"};
}
}  // namespace stonks