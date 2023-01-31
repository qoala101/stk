#ifndef STONKS_NETWORK_NETWORK_CONCEPTS_H_
#define STONKS_NETWORK_NETWORK_CONCEPTS_H_

#include "network_i_json.h"
#include "network_json_base_conversions.h"

/**
 * @file Common network library concepts.
 */

namespace vh::network {
template <typename T>
concept Parsable = requires(const IJson &json) {
                     { JsonParser<T>{}(json) } -> std::same_as<T>;
                   };

// TODO(vh): Find requirements for the concept. The one below doesn't work,
// because available declarations are searched only among the ones visible here,
// meaning that user-defined conversions would not be visible here.
template <typename T>
concept Convertible = !
std::same_as<T, void>;
}  // namespace vh::network

#endif  // STONKS_NETWORK_NETWORK_CONCEPTS_H_
