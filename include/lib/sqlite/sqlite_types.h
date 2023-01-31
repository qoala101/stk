#ifndef VH_SQLITE_TYPES_H_
#define VH_SQLITE_TYPES_H_

#include <string>

#include "cpp_typed_struct.h"

namespace vh::sqlite {
/**
 * @brief Path to DB file.
 */
struct FilePath : public cpp::TypedStruct<std::string> {};

/**
 * @brief SQLite result code.
 */
struct ResultCode : public cpp::TypedStruct<int> {};
}  // namespace vh::sqlite

#endif  // VH_SQLITE_TYPES_H_
