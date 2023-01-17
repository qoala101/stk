#ifndef STONKS_CPP_CPP_THREAD_SAFE_TAG_H_
#define STONKS_CPP_CPP_THREAD_SAFE_TAG_H_

#include "cpp_typed_struct.h"

namespace stonks::cpp {
/**
 * @brief Used to explicitly mark variables that are expected to be thread safe
 * so that clients can provide specific implementations.
 */
template <typename T>
struct ThreadSafe : public cpp::TypedStruct<T> {};
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_THREAD_SAFE_TAG_H_
