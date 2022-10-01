#ifndef STONKS_CORE_CORE_TYPES_H_
#define STONKS_CORE_CORE_TYPES_H_

#include <string>

#include "cpp_typed_struct.h"

namespace stonks::core {
/**
 * @brief Name of the pair of assets.
 */
struct Symbol : public cpp::TypedStruct<std::string> {};
}  // namespace stonks::core

#endif  // STONKS_CORE_CORE_TYPES_H_
