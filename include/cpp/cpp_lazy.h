#ifndef STONKS_CPP_CPP_LAZY_H_
#define STONKS_CPP_CPP_LAZY_H_

#include <function2/function2.hpp>
#include <gsl/assert>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_not_null.h"
#include "cpp_this.h"  // IWYU pragma: keep

namespace stonks::cpp {
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
  template <cpp::InvocableReturning<T> Initializer>
  // NOLINTNEXTLINE(*-forwarding-reference-overload)
  explicit Lazy(Initializer &&initializer)
      : initializer_{std::forward<Initializer>(initializer)} {
    Ensures(!initializer_.empty());
  }

  /**
   * @brief Gives lazy initialized object.
   */
  auto GetObject [[nodiscard]] () const -> auto & {
    return GetObjectImpl(*this);
  }

  auto GetObject [[nodiscard]] () -> auto & { return GetObjectImpl(*this); }

  /**
   * @brief Gives an object the lazy initialized one points to.
   */
  auto operator* [[nodiscard]] () const -> auto & {
    return OperatorAsteriskImpl(*this);
  }

  auto operator* [[nodiscard]] () -> auto & {
    return OperatorAsteriskImpl(*this);
  }

  /**
   * @brief Gives an object the lazy initialized one points to.
   */
  auto operator->[[nodiscard]] () const { return OperatorArrowImpl(*this); }

  auto operator->[[nodiscard]] () { return OperatorArrowImpl(*this); }

 private:
  static auto GetObjectImpl [[nodiscard]] (This<Lazy> auto &t) -> auto & {
    if (!t.object_.has_value()) {
      t.object_ = t.initializer_();
    }

    Ensures(t.object_.has_value());
    return *t.object_;
  }

  static auto OperatorAsteriskImpl [[nodiscard]] (This<Lazy> auto &t)
  -> auto & {
    auto &object = t.GetObject();

    if constexpr (PointerLike<T>) {
      return *object;
    } else {
      return object;
    }
  }

  static auto OperatorArrowImpl [[nodiscard]] (This<Lazy> auto &t) {
    return AssumeNn(&*t);
  }

  mutable fu2::unique_function<T()> initializer_{};
  mutable Opt<T> object_{};
};
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_LAZY_H_
