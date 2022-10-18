#ifndef STONKS_CPP_CPP_LAZY_H_
#define STONKS_CPP_CPP_LAZY_H_

#include <function2/function2.hpp>
#include <gsl/assert>

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
  explicit Lazy(fu2::unique_function<T()> initializer)
      : initializer_{std::move(initializer)} {
    Expects(!initializer_.empty());
  }

  /**
   * @brief Gives lazy initialized object.
   */
  [[nodiscard]] auto GetObject() const -> auto & {
    return GetObjectImpl(*this);
  }

  [[nodiscard]] auto GetObject() -> auto & { return GetObjectImpl(*this); }

  /**
   * @brief Gives an object the lazy initialized one points to.
   */
  [[nodiscard]] auto operator->() const { return OperatorArrowImpl(*this); }

  [[nodiscard]] auto operator->() { return OperatorArrowImpl(*this); }

  /**
   * @brief Gives an object the lazy initialized one points to.
   */
  [[nodiscard]] auto operator*() const -> auto & {
    return OperatorAsteriskImpl(*this);
  }

  [[nodiscard]] auto operator*() -> auto & {
    return OperatorAsteriskImpl(*this);
  }

 private:
  [[nodiscard]] static auto GetObjectImpl(This<Lazy> auto &t) -> auto & {
    if (!t.object_.has_value()) {
      t.object_ = t.initializer_();
    }

    Ensures(t.object_.has_value());
    return *t.object_;
  }

  [[nodiscard]] static auto OperatorAsteriskImpl(This<Lazy> auto &t) -> auto & {
    auto &object = t.GetObject();

    if constexpr (PointerLike<T>) {
      return *object;
    } else {
      return object;
    }
  }

  [[nodiscard]] static auto OperatorArrowImpl(This<Lazy> auto &t) {
    return AssumeNn(&*t);
  }

  mutable fu2::unique_function<T()> initializer_{};
  mutable Opt<T> object_{};
};
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_LAZY_H_
