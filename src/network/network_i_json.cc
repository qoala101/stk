#include "network_i_json.h"

#include <utility>

namespace stonks::network {
void IJson::SetChild(std::string key, IJson&& child) {
  return SetChild(std::move(key), child);
}
}  // namespace stonks::network