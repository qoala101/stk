#ifndef STONKS_CPP_CPP_LAZY_H_
#define STONKS_CPP_CPP_LAZY_H_

#include <function2/function2.hpp>
#include <gsl/assert>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_not_null.h"
#include "cpp_this.h"  // IWYU pragma: keep

namespace vh::cpp {
/**
 * @brief Lazy initialized object.
 */
template <typename T>
class Lazy {
 public:
  /**
   * @param initializer Would be called to create the object
   * when it's requested the first time.
   */
  template <CallableReturning<T> Initializer>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit Lazy(Initializer &&initializer)
      : initializer_{std::forward<Initializer>(initializer)} {
    Ensures(!initializer_.empty());
  }

  /**
   * @brief Gives lazy initialized object.
   */
  auto GetObject [[nodiscard]] () -> auto & {
    if (!object_.has_value()) {
      object_ = initializer_();
    }

    Ensures(object_.has_value());
    return *object_;
  }

  /**
   * @brief Gives tne object the lazy initialized one points to.
   */
  auto operator* [[nodiscard]] () -> auto & {
    auto &object = GetObject();

    if constexpr (PointerLike<T>) {
      return *object;
    } else {
      return object;
    }
  }

  /**
   * @brief Gives the object the lazy initialized one points to.
   */
  auto operator->[[nodiscard]] () { return AssumeNn(&*(*this)); }

 private:
  fu2::unique_function<auto()->T> initializer_{};
  Opt<T> object_{};
};
}  // namespace vh::cpp

#endif  // STONKS_CPP_CPP_LAZY_H_
