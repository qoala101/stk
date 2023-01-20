#include "cpp_lock.h"

namespace stonks::cpp {
Lock::Lock(NnSp<std::mutex> mutex) : mutex_{std::move(mutex)}, lock_{*mutex_} {}
}  // namespace stonks::cpp