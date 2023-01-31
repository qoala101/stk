#ifndef VH_CPP_META_THREAD_SAFE_H_
#define VH_CPP_META_THREAD_SAFE_H_

#include "cpp_typed_struct.h"

namespace vh::cpp::meta {
/**
 * @brief Used to explicitly mark variables that are expected to be thread safe
 * so that clients can provide specific implementations.
 */
template <typename T>
struct ThreadSafe : public TypedStruct<T> {
 private:
  template <typename U>
  friend auto AssumeThreadSafe(U t);

  explicit ThreadSafe(T t) : TypedStruct<T>{std::move(t)} {}
};

/**
 * @brief An explicit way for the client to tell that variable is thread safe.
 */
template <typename T>
auto AssumeThreadSafe(T t) {
  return ThreadSafe<T>{std::move(t)};
}
}  // namespace vh::cpp::meta

#endif  // VH_CPP_META_THREAD_SAFE_H_
