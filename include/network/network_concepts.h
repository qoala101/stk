#ifndef STONKS_NETWORK_NETWORK_CONCEPTS_H_
#define STONKS_NETWORK_NETWORK_CONCEPTS_H_

#include "network_i_json.h"
#include "network_json_base_conversions.h"

/**
 * @file Common network library concepts.
 */

namespace stonks::network {
template <typename T>
concept Parsable = requires(const IJson &json) {
                     { JsonParser<T>{}(json) } -> std::same_as<T>;
                   };

template <typename T>
concept Convertible = requires(const T &t) {
                        true;
                        // TODO(vh): Find requirements for the concept. The one
                        // below doesn't work, because available declarations
                        // are searched only among the ones visible here,
                        // meaning that user-defined conversions would not be
                        // visible here. { ConvertToJson(t) } ->
                        // std::same_as<cpp::Pv<IJson>>;
                      };
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_CONCEPTS_H_
