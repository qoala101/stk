/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "cpp_lock.h"

#include <gsl/assert>
#include <not_null.hpp>
#include <utility>

namespace vh::cpp {
Lock::Lock(NnSp<std::mutex> mutex) : mutex_{std::move(mutex)}, lock_{*mutex_} {
  Ensures(lock_.owns_lock());
}
}  // namespace vh::cpp