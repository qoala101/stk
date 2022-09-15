#ifndef STONKS_SQLITE_SQLITE_DB_HANDLES_FACTORY_H_
#define STONKS_SQLITE_SQLITE_DB_HANDLES_FACTORY_H_

#include <string_view>

#include "cpp_not_null.h"
#include "log_i_logger.h"
#include "sqlite_raw_handles.h"

/**
 * @file Constructors of SQLite handles.
 */

namespace stonks::sqlite {
class DbHandlesFactory {
 public:
  explicit DbHandlesFactory(cpp::NnSp<log::ILogger> logger);

  /**
   * @brief Creates in-memory SQLite DB.
   */
  [[nodiscard]] auto CreateInMemoryDb() const -> SqliteDbHandle;

  /**
   * @brief Creates SQLite handle to DB from file.
   */
  [[nodiscard]] auto CreateHandleToFileDb(std::string_view file_path) const
      -> SqliteDbHandle;

  /**
   * @brief Reads SQLite DB from file and copies the contents to the in-memory
   * DB.
   * @return Handle to in-memory DB.
   */
  [[nodiscard]] auto LoadDbFromFileToMemory(std::string_view file_path) const
      -> SqliteDbHandle;

 private:
  cpp::NnSp<log::ILogger> logger_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_HANDLES_FACTORY_H_
