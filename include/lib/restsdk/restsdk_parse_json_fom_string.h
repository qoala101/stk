/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_RESTSDK_PARSE_JSON_FOM_STRING_H_
#define VH_RESTSDK_PARSE_JSON_FOM_STRING_H_

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

#endif  // VH_RESTSDK_PARSE_JSON_FOM_STRING_H_
