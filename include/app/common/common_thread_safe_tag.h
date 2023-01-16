#ifndef STONKS_APP_COMMON_COMMON_THREAD_SAFE_TAG_H_
#define STONKS_APP_COMMON_COMMON_THREAD_SAFE_TAG_H_

#include "cpp_typed_struct.h"

namespace stonks::common {
/**
 * @brief Used to explicitly mark variables that are expected to be thread safe
 * so that clients can provide specific implementations.
 */
template <typename T>
struct ThreadSafe : public cpp::TypedStruct<T> {};
}  // namespace stonks::common

#endif  // STONKS_APP_COMMON_COMMON_THREAD_SAFE_TAG_H_
