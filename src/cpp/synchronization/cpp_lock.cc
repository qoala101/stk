#include "cpp_lock.h"

#include <gsl/assert>

namespace stonks::cpp {
Lock::Lock(NnSp<std::mutex> mutex) : mutex_{std::move(mutex)}, lock_{*mutex_} {
  Ensures(lock_.owns_lock());
}
}  // namespace stonks::cpp