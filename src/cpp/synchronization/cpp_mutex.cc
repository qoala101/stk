#include "cpp_mutex.h"

namespace stonks::cpp {
Mutex::Mutex() : mutex_{cpp::MakeNnSp<std::mutex>()} {}

auto Mutex::Lock() const -> LockVariant {
  return LockVariant{std::in_place_type<class Lock>, mutex_};
}

auto MutexVariant::Lock() const -> LockVariant {
  return std::visit(
      [](auto &v) -> LockVariant {
        using V = std::remove_cvref_t<decltype(v)>;

        if constexpr (std::is_same_v<V, Mutex>) {
          return v.Lock();
        }

        return {std::monostate{}};
      },
      **this);
}
}  // namespace stonks::cpp