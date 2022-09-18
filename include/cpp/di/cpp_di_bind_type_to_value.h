#ifndef STONKS_CPP_DI_CPP_DI_BIND_TYPE_TO_VALUE_H_
#define STONKS_CPP_DI_CPP_DI_BIND_TYPE_TO_VALUE_H_

#include <boost/di.hpp>

namespace stonks::cpp::di {
/**
 * @brief Binds type to value.
 */
template <typename Type, typename Value>
[[nodiscard]] auto BindTypeToValue(Value &&value) {
  return boost::di::bind<Type>().to(std::forward<Value>(value));
}
}  // namespace stonks::cpp::di

#endif  // STONKS_CPP_DI_CPP_DI_BIND_TYPE_TO_VALUE_H_
