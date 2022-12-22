#ifndef STONKS_DI_DI_AUTO_INJECTABLE_H_
#define STONKS_DI_DI_AUTO_INJECTABLE_H_

#include "cpp_not_null.h"

namespace stonks::di {
/**
 * @brief Automatically converts to any type created by injector.
 */
template <typename Injector>
class AutoInjectable {
 public:
  explicit AutoInjectable(cpp::Nn<const Injector *> injector)
      : injector_{injector} {}

  /**
   * @tparam T Type to be created by injector.
   */
  template <typename T>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  [[nodiscard]] operator T() {
    return injector_->template create<T>();
  }

 private:
  cpp::Nn<const Injector *> injector_;
};
}  // namespace stonks::di

#endif  // STONKS_DI_DI_AUTO_INJECTABLE_H_
