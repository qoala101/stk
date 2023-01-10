#ifndef STONKS_CPP_CPP_AUTO_UPDATABLE_H_
#define STONKS_CPP_CPP_AUTO_UPDATABLE_H_

#include <absl/time/clock.h>
#include <absl/time/time.h>

#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <function2/function2.hpp>
#include <gsl/assert>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"

namespace stonks::cpp {
namespace detail {
struct UpdateLock {};

class AutoUpdatableBase {
 public:
  /**
   * @brief Prevents value from being updated in scope.
   */
  auto LockUpdates [[nodiscard]] () -> cpp::Sp<UpdateLock>;

 protected:
  explicit AutoUpdatableBase(absl::Duration update_interval);

  auto AreUpdatesLocked [[nodiscard]] () const -> bool;

  auto IsTimeToUpdate [[nodiscard]] () const -> bool;

 private:
  absl::Duration update_interval_{};
  absl::Time last_update_time_{};
  cpp::Wp<detail::UpdateLock> update_lock_;
};
}  // namespace detail

/**
 * @brief Automatically updated object.
 */
template <typename T>
class AutoUpdatable : public detail::AutoUpdatableBase {
 public:
  /**
   * @param updater Would be called to create the object
   * when it's requested the first time and then every time when requested after
   * the specified interval.
   */
  template <cpp::CallableReturning<cppcoro::task<T>> Updater>
  AutoUpdatable(Updater &&updater, absl::Duration update_interval)
      : detail::AutoUpdatableBase{update_interval},
        updater_{std::forward<Updater>(updater)},
        object_{cppcoro::sync_wait(updater_())} {
    Ensures(!updater_.empty());
  }

  /**
   * @brief Gives the updated object.
   */
  auto GetObject [[nodiscard]] () -> cppcoro::task<T &> {
    if (IsTimeToUpdate() && !AreUpdatesLocked()) {
      object_ = co_await updater_();
    }

    co_return object_;
  }

  /**
   * @brief Gives the object the updated one points to.
   */
  auto operator* [[nodiscard]] () -> cppcoro::task<T &> {
    auto &object = co_await GetObject();

    if constexpr (PointerLike<T>) {
      co_return *object;
    } else {
      co_return object;
    }
  }

  /**
   * @brief Gives the object the updated one points to.
   */
  auto operator->[[nodiscard]] () -> cppcoro::task<cpp::Nn<T *>> {
    co_return AssumeNn(&*(co_await (*this)));
  }

 private:
  fu2::unique_function<auto()->cppcoro::task<T>> updater_{};
  T object_{};
};
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_AUTO_UPDATABLE_H_
