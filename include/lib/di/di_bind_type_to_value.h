#ifndef STONKS_DI_DI_BIND_TYPE_TO_VALUE_H_
#define STONKS_DI_DI_BIND_TYPE_TO_VALUE_H_

#include <boost/di.hpp>

namespace vh::di {
/**
 * @brief Binds type to value.
 */
template <typename T, typename Value>
auto BindTypeToValue [[nodiscard]] (Value &&value) {
  return boost::di::bind<T>().to(std::forward<Value>(value));
}
}  // namespace vh::di

#endif  // STONKS_DI_DI_BIND_TYPE_TO_VALUE_H_
