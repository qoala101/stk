#ifndef STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_JSON_USER_CONVERSIONS_H_
#define STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_JSON_USER_CONVERSIONS_H_

#include "cpp_polymorphic_value.h"
#include "network_i_json.h"
#include "nosqldb_types.h"

namespace stonks::network {
template <typename T>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> T;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> nosqldb::Item;
}  // namespace stonks::network

namespace stonks::nosqldb {
[[nodiscard]] auto ConvertToJson(const Item &value) -> cpp::Pv<network::IJson>;
}  // namespace stonks::nosqldb

#endif  // STONKS_APP_PUBLIC_NOSQL_DB_APP_PND_JSON_USER_CONVERSIONS_H_