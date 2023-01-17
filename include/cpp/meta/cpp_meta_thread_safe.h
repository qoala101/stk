#ifndef STONKS_CPP_META_CPP_META_THREAD_SAFE_TAG_H_
#define STONKS_CPP_META_CPP_META_THREAD_SAFE_TAG_H_

#include "cpp_typed_struct.h"

namespace stonks::cpp::meta {
/**
 * @brief Used to explicitly mark variables that are expected to be thread safe
 * so that clients can provide specific implementations.
 */
template <typename T>
struct ThreadSafe : public cpp::TypedStruct<T> {};
}  // namespace stonks::cpp::meta

#endif  // STONKS_CPP_META_CPP_META_THREAD_SAFE_TAG_H_
