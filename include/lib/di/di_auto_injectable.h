#ifndef STONKS_DI_DI_AUTO_INJECTABLE_H_
#define STONKS_DI_DI_AUTO_INJECTABLE_H_

#include "cpp_not_null.h"

namespace vh::di {
/**
 * @brief Automatically converts to any type created by injector.
 */
template <typename Injector>
class AutoInjectable {
 public:
  explicit AutoInjectable(cpp::NnSp<Injector> injector)
      : injector_{std::move(injector)} {}

  /**
   * @tparam T Type to be created by injector.
   */
  template <typename T>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  [[nodiscard]] operator T() {
    return injector_->template create<T>();
  }

 private:
  cpp::NnSp<Injector> injector_;
};
}  // namespace vh::di

#endif  // STONKS_DI_DI_AUTO_INJECTABLE_H_
