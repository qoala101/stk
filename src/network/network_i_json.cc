#include "network_i_json.h"

namespace stonks::network {
void IJson::SetChild(std::string_view key, IJson&& child) {
  return SetChild(key, child);
}
}  // namespace stonks::network