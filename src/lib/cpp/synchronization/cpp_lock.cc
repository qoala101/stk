#include "cpp_lock.h"

#include <gsl/assert>
#include <not_null.hpp>
#include <utility>

namespace vh::cpp {
Lock::Lock(NnSp<std::mutex> mutex) : mutex_{std::move(mutex)}, lock_{*mutex_} {
  Ensures(lock_.owns_lock());
}
}  // namespace vh::cpp