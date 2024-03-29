/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CPP_MUTEX_H_
#define VH_CPP_MUTEX_H_

#include <mutex>
#include <variant>

#include "cpp_lock.h"
#include "cpp_not_null.h"
#include "cpp_variant_struct.h"

namespace vh::cpp {
/**
 * @brief Mutex which is shared with created locks.
 */
class Mutex {
 public:
  Mutex();

  Mutex(const Mutex &) = delete;
  Mutex(Mutex &&) noexcept = default;

  auto operator=(const Mutex &) -> Mutex & = delete;
  auto operator=(Mutex &&) noexcept -> Mutex & = default;

  ~Mutex() = default;

  /**
   * @brief
   */
  auto Lock [[nodiscard]] () const -> LockVariant;

 private:
  NnSp<std::mutex> mutex_;
};

/**
 * @brief Mutex variant which does nothing by default.
 */
struct MutexVariant : public VariantStruct<std::monostate, Mutex> {
  /**
   * @brief Locks the mutex if it has value.
   */
  auto Lock [[nodiscard]] () const -> LockVariant;
};
}  // namespace vh::cpp

#endif  // VH_CPP_MUTEX_H_
