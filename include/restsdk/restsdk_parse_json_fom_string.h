#ifndef STONKS_RESTSDK_RESTSDK_PARSE_JSON_FOM_STRING_H_
#define STONKS_RESTSDK_RESTSDK_PARSE_JSON_FOM_STRING_H_

#include <string>

#include "cpp_polymorphic_value.h"
#include "network_i_json.h"

namespace stonks::restsdk {
/**
 * @brief Parses JSON from a raw string.
 */
[[nodiscard]] auto ParseJsonFromString(const std::string &value)
    -> cpp::Pv<network::IJson>;
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_PARSE_JSON_FOM_STRING_H_
