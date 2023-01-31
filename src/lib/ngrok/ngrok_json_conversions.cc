#include <vector>

#include "network_i_json.h"
#include "network_json_base_conversions.h"
#include "network_json_common_conversions.h"
#include "network_json_conversions_facades.h"
#include "ngrok_types.h"

namespace vh::network {
template <>
auto JsonParser<ngrok::Tunnel>::operator()(const IJson &json) const -> Type {
  return MakeFromJson<Type>(json, "public_url");
}

template <>
auto JsonParser<ngrok::Tunnels>::operator()(const IJson &json) const -> Type {
  return {ParseFromJsonChild<std::vector<ngrok::Tunnel>>(json, "tunnels")};
}
}  // namespace vh::network
