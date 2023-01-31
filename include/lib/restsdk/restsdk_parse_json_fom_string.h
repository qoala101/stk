#ifndef STONKS_RESTSDK_RESTSDK_PARSE_JSON_FOM_STRING_H_
#define STONKS_RESTSDK_RESTSDK_PARSE_JSON_FOM_STRING_H_

#include <string>

#include "cpp_polymorphic_value.h"
#include "network_i_json.h"

namespace vh::restsdk {
/**
 * @brief Parses JSON from a raw string.
 */
auto ParseJsonFromString [[nodiscard]] (const std::string &value)
-> cpp::Pv<network::IJson>;
}  // namespace vh::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_PARSE_JSON_FOM_STRING_H_
