#ifndef STONKS_NETWORK_NETWORK_JSON_SPECIALIZED_CONVERSIONS_H_
#define STONKS_NETWORK_NETWORK_JSON_SPECIALIZED_CONVERSIONS_H_

#include <polymorphic_value.h>

#include <exception>
#include <stdexcept>
#include <type_traits>

#include "cpp_concepts.h"
#include "cpp_message_exception.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "network_i_json.h"
#include "network_json_basic_conversions.h"
#include "not_null.hpp"

/**
 * @file Conversions of standard templates.
 */

namespace stonks::network {
template <cpp::Optional T>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> T {
  if (IsNullJson(json)) {
    return std::nullopt;
  }

  return ParseFromJson<typename T::value_type>(json);
}

template <typename T>
[[nodiscard]] auto ConvertToJson(T *value) -> cpp::Pv<IJson> {
  if (value == nullptr) {
    return CreateNullJson();
  }

  return ConvertToJson(*value);
}
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_SPECIALIZED_CONVERSIONS_H_
