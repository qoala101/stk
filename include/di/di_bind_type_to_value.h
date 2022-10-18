#ifndef STONKS_DI_DI_BIND_TYPE_TO_VALUE_H_
#define STONKS_DI_DI_BIND_TYPE_TO_VALUE_H_

#include <boost/di.hpp>

namespace stonks::di {
/**
 * @brief Binds type to value.
 */
template <typename T, typename Value>
[[nodiscard]] auto BindTypeToValue(Value &&value) {
  return boost::di::bind<T>().to(std::forward<Value>(value));
}
}  // namespace stonks::di

#endif  // STONKS_DI_DI_BIND_TYPE_TO_VALUE_H_
