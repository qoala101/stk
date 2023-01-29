#include "cpp_auto_updatable.h"

namespace stonks::cpp::detail {
auto AutoUpdatableBase::LockUpdates [[nodiscard]] () -> Sp<UpdateLock> {
  auto update_lock = update_lock_.lock();

  if (update_lock == nullptr) {
    update_lock = MakeSp<detail::UpdateLock>();
    update_lock_ = update_lock;
  }

  Ensures(AreUpdatesLocked());
  return update_lock;
}

AutoUpdatableBase::AutoUpdatableBase(absl::Duration update_interval)
    : update_interval_{update_interval} {}

auto AutoUpdatableBase::AreUpdatesLocked [[nodiscard]] () const -> bool {
  return !update_lock_.expired();
}

auto AutoUpdatableBase::IsTimeToUpdate [[nodiscard]] () const -> bool {
  const auto time_since_last_update = absl::Now() - last_update_time_;
  return time_since_last_update >= update_interval_;
}
}  // namespace stonks::cpp::detail