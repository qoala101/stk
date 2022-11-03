#ifndef STONKS_CPP_ALIASES_CPP_TYPE_LIST_H_
#define STONKS_CPP_ALIASES_CPP_TYPE_LIST_H_

#include <tuple>

namespace stonks::cpp {
/**
 * @brief Used to represent the list of types.
 */
template <typename... Ts>
using TypeList = std::tuple<Ts...>;
}  // namespace stonks::cpp

#endif  // STONKS_CPP_ALIASES_CPP_TYPE_LIST_H_
