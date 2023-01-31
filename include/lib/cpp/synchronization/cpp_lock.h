#ifndef VH_CPP_LOCK_H_
#define VH_CPP_LOCK_H_

#include <mutex>
#include <variant>

#include "cpp_not_null.h"

namespace vh::cpp {
/**
 * @brief Lock which shares mutex ownership.
 */
class Lock {
 public:
  /**
   * @brief Locks the mutex.
   */
  explicit Lock(NnSp<std::mutex> mutex);

 private:
  NnSp<std::mutex> mutex_;
  std::unique_lock<std::mutex> lock_{};
};

/**
 * @brief Lock variant which does nothing by default.
 */
using LockVariant = std::variant<std::monostate, Lock>;
}  // namespace vh::cpp

#endif  // VH_CPP_LOCK_H_
