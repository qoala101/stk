#ifndef VH_CPP_TYPE_LIST_H_
#define VH_CPP_TYPE_LIST_H_

#include <tuple>

namespace vh::cpp {
/**
 * @brief Used to represent the list of types.
 */
template <typename... Ts>
using TypeList = std::tuple<Ts...>;
}  // namespace vh::cpp

#endif  // VH_CPP_TYPE_LIST_H_
