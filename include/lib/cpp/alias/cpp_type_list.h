#ifndef STONKS_CPP_ALIAS_CPP_TYPE_LIST_H_
#define STONKS_CPP_ALIAS_CPP_TYPE_LIST_H_

#include <tuple>

namespace vh::cpp {
/**
 * @brief Used to represent the list of types.
 */
template <typename... Ts>
using TypeList = std::tuple<Ts...>;
}  // namespace vh::cpp

#endif  // STONKS_CPP_ALIAS_CPP_TYPE_LIST_H_
