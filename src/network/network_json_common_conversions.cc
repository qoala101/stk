#include "network_json_common_conversions.h"

#include <string>
#include <string_view>

#include "network_json_facades_aliases_p_c.h"

namespace stonks::network {
auto ConvertToJson(const char *value) -> cpp::Pv<IJson> {
  return C(std::string_view{value});
}

template <>
auto ParseFromJson(const IJson &json) -> cpp::MessageException {
  return cpp::MessageException{P<std::string>(*json.GetChild("message"))};
}

auto ConvertToJson(const std::exception &value) -> cpp::Pv<IJson> {
  return C("message", value.what());
}
}  // namespace stonks::network