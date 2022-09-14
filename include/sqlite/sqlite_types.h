#ifndef STONKS_SQLITE_SQLITE_TYPES_H_
#define STONKS_SQLITE_SQLITE_TYPES_H_

#include "cpp_typed_struct.h"

namespace stonks::sqlite {
/**
 * @brief SQLite result code.
 */
struct ResultCode : public cpp::TypedStruct<int> {};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_TYPES_H_
