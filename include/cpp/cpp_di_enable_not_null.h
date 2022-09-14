#ifndef STONKS_CPP_CPP_DI_ENABLE_NOT_NULL_H_
#define STONKS_CPP_CPP_DI_ENABLE_NOT_NULL_H_

#include <boost/di.hpp>

#include "cpp_not_null.h"

namespace stonks::cpp::di {
namespace detail {
template <typename T>
struct AssumedNn : public Nn<T> {
  explicit AssumedNn(T t) : Nn<T>{AssumeNn(std::move(t))} {}
};
}  // namespace detail

/**
 * @brief Enables not null injection for the type.
 * @remark Non null doesn't have a constructor which can be used by Boost DI
 * and using runtime injection doesn't work for const references.
 * This approach solves both problems.
 */
template <typename T>
auto EnableNn() {
  return ::boost::di::bind<Nn<T>>().template to<detail::AssumedNn<T>>();
}
}  // namespace stonks::cpp::di

#endif  // STONKS_CPP_CPP_DI_ENABLE_NOT_NULL_H_
