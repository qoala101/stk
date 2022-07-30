#include "network_i_json.h"

namespace stonks::network {
void IJson::SetChild(std::string key, IJson&& child) {
  return SetChild(std::move(key), child);
}
}  // namespace stonks::network