/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CPP_FACTORY_H_
#define VH_CPP_FACTORY_H_

#include <function2/function2.hpp>
#include <gsl/assert>
#include <type_traits>

#include "cpp_concepts.h"
#include "cpp_mutex.h"
#include "cpp_not_null.h"

namespace vh::cpp {
namespace detail {
template <typename T>
struct FactoryTraits {
  using ResultType = T;
};

template <Interface T>
struct FactoryTraits<T> {
  using ResultType = NnUp<T>;
};
}  // namespace detail

/**
 * @brief Simple factory interface.
 */
template <typename T>
class Factory {
 public:
  using ResultType = typename detail::FactoryTraits<T>::ResultType;

  /**
   * @param creator Would be called to create the objects.
   */
  template <CallableReturning<ResultType> Creator>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  Factory(Creator &&creator, MutexVariant create_mutex)
      : creator_{std::forward<Creator>(creator)},
        create_mutex_{std::move(create_mutex)} {
    Ensures(!creator_.empty());
  }

  /**
   * @brief Creates default constructible objects.
   */
  explicit Factory(MutexVariant create_mutex)
      : Factory{[]() { return T{}; }, std::move(create_mutex)} {}

  /**
   * @brief Creates the object.
   */
  auto Create [[nodiscard]] () {
    const auto lock = create_mutex_.Lock();
    return creator_();
  }

 private:
  fu2::function<auto()->ResultType> creator_{};
  MutexVariant create_mutex_{};
};
}  // namespace vh::cpp

#endif  // VH_CPP_FACTORY_H_
