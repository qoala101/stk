#ifndef STONKS_SQLITE_SQLITE_DB_HANDLES_FACTORY_H_
#define STONKS_SQLITE_SQLITE_DB_HANDLES_FACTORY_H_

#include "cpp_not_null.h"
#include "di_factory.h"
#include "log_i_logger.h"
#include "sqlite_raw_handles.h"
#include "sqlite_types.h"

/**
 * @file Constructors of SQLite handles.
 */

namespace stonks::sqlite {
class DbHandlesFactory {
 public:
  explicit DbHandlesFactory(di::Factory<log::ILogger> logger_factory);

  /**
   * @brief Creates in-memory SQLite DB.
   */
  [[nodiscard]] auto CreateInMemoryDb() const -> SqliteDbHandle;

  /**
   * @brief Creates SQLite handle to DB from file.
   */
  [[nodiscard]] auto CreateHandleToFileDb(const FilePath &file_path) const
      -> SqliteDbHandle;

  /**
   * @brief Reads SQLite DB from file and copies the contents to the in-memory
   * DB.
   * @return Handle to in-memory DB.
   */
  [[nodiscard]] auto LoadDbFromFileToMemory(const FilePath &file_path) const
      -> SqliteDbHandle;

 private:
  di::Factory<log::ILogger> logger_factory_;
  cpp::NnUp<log::ILogger> logger_;
};
}  // namespace stonks::sqlite

#endif  // STONKS_SQLITE_SQLITE_DB_HANDLES_FACTORY_H_
