#ifndef STONKS_CPP_CPP_FACTORY_H_
#define STONKS_CPP_CPP_FACTORY_H_

#include <function2/function2.hpp>
#include <gsl/assert>
#include <type_traits>

#include "cpp_concepts.h"
#include "cpp_not_null.h"

namespace stonks::cpp {
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

template <typename T>
/**
 * @brief Simple factory interface.
 */
class Factory {
 public:
  using ResultType = typename detail::FactoryTraits<T>::ResultType;

  /**
   * @param creator Would be called to create the objects.
   */
  template <cpp::CallableReturning<ResultType> Creator>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit Factory(Creator &&creator)
      : creator_{std::forward<Creator>(creator)} {
    Ensures(!creator_.empty());
  }

  /**
   * @brief Creates the object.
   */
  auto Create [[nodiscard]] () { return creator_(); }

 private:
  fu2::unique_function<auto()->ResultType> creator_{};
};
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_FACTORY_H_
