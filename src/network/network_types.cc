#include "network_types.h"

namespace stonks::network {
auto RestRequest::IsEmpty() const -> bool {
  return endpoint.uri.empty() && (endpoint.method == Method{}) &&
         params.empty() && headers.empty() && !body.has_value();
}
}  // namespace stonks::network