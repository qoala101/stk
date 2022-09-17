#include "app_pnd_json_user_conversions.h"

#include "network_json_conversions_facades.h"
#include "nosqldb_types.h"

namespace stonks::network {
template <>
auto ParseFromJson(const IJson &json) -> nosqldb::Item {
  return MakeFromJson<nosqldb::Item>(json, "key", "value");
}
}  // namespace stonks::network

namespace stonks::nosqldb {
auto ConvertToJson(const Item &value) -> cpp::Pv<network::IJson> {
  return network::BuildJsonFrom("key", value.key, "value", value.value);
}
}  // namespace stonks::nosqldb
