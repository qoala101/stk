#include "sqlite_db.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <sqlite3.h>

#include <functional>
#include <gsl/pointers>
#include <gsl/util>
#include <magic_enum.hpp>
#include <map>
#include <memory>
#include <optional>
#include <range/v3/action/erase.hpp>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/algorithm/remove_if.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/dangling.hpp>
#include <range/v3/view/adaptor.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/view.hpp>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "db_enum_conversions.h"  // IWYU pragma: keep
#include "db_prepared_statement.h"
#include "db_types.h"
#include "sqlite_prepared_statement.h"

namespace stonks::db::sqlite {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = []() {
    auto logger = spdlog::stdout_color_mt("Client");
    logger->set_level(spdlog::level::debug);
    return logger;
  }();

  return *logger;
}

[[nodiscard]] auto GetDbFileName(sqlite3 &sqlite_db) -> std::string {
  const auto *const file_name = sqlite3_db_filename(&sqlite_db, nullptr);

  if (file_name == nullptr) {
    return {};
  }

  return file_name;
}

[[nodiscard]] auto OpenSqliteDb(std::string_view file_name)
    -> gsl::not_null<sqlite3 *> {
  auto *sqlite_db = (sqlite3 *){};
  sqlite3_open(file_name.data(), &sqlite_db);

  if (sqlite_db == nullptr) {
    throw std::runtime_error{std::string{"Cannot open DB from "} +
                             file_name.data() + ": " +
                             sqlite3_errmsg(sqlite_db)};
  }

  return sqlite_db;
}

void SetPragmas(sqlite3 &sqlite_db) {
  const auto result_code = sqlite3_exec(&sqlite_db, "PRAGMA foreign_keys = ON",
                                        nullptr, nullptr, nullptr);

  if (result_code != SQLITE_OK) {
    Logger().error("Failed to enable foreign key checks");
  }
}

[[nodiscard]] auto PrepareSqliteStatement(sqlite3 &sqlite_db,
                                          std::string_view query)
    -> gsl::not_null<sqlite3_stmt *> {
  auto *sqlite_statement = (sqlite3_stmt *){};
  sqlite3_prepare_v3(&sqlite_db, query.data(),
                     static_cast<int>(query.length()) + 1,
                     SQLITE_PREPARE_PERSISTENT, &sqlite_statement, nullptr);

  if (sqlite_statement == nullptr) {
    throw std::runtime_error{
        std::string{"Cannot prepare statement for query: "} + query.data()};
  }

  return sqlite_statement;
}

void RemoveStatement(std::vector<std::shared_ptr<sqlite3_stmt>> &statements,
                     sqlite3_stmt &sqlite_statement) {
  ranges::erase(
      statements,
      ranges::remove_if(statements,
                        [&sqlite_statement](const auto &prepared_statement) {
                          return &(*prepared_statement) == &sqlite_statement;
                        }),
      statements.end());
}

void CloseSqliteDb(sqlite3 &sqlite_db) {
  const auto file_name = GetDbFileName(sqlite_db);
  const auto result_code = sqlite3_close(&sqlite_db);

  if (result_code != SQLITE_OK) {
    Logger().error("Cannot close DB from {}", file_name);
    return;
  }

  Logger().info("Closed DB from {}", file_name);
}

void ExecuteQuery(sqlite3 &sqlite_db, std::string_view query,
                  int (*callback)(void *, int, char **, char **) = nullptr,
                  void *data = nullptr) {
  auto *error_message = (char *){};
  const auto finally_free_error_message = gsl::finally([&error_message]() {
    if (error_message != nullptr) {
      sqlite3_free(error_message);
    }
  });

  Logger().debug("Executing query: {}", query);
  const auto result_code =
      sqlite3_exec(&sqlite_db, query.data(), callback, data, &error_message);

  if (result_code != SQLITE_OK) {
    throw std::runtime_error{std::string{"Couldn't execute query: "} +
                             ((error_message != nullptr)
                                  ? error_message
                                  : std::to_string(result_code))};
  }
}
}  // namespace

class SqliteDb::Impl {
 public:
  Impl() : Impl(":memory:") {}

  explicit Impl(std::string_view file_name)
      : sqlite_db_{OpenSqliteDb(file_name)} {
    SetPragmas(*sqlite_db_);
    Logger().info("Opened DB from {}", GetDbFileName(*sqlite_db_));
  }

  Impl(const Impl &) = delete;
  Impl(Impl &&) = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) -> Impl & = default;

  ~Impl() {
    prepared_statements_.clear();
    CloseSqliteDb(*sqlite_db_);
  }

  void CreateTableIfNotExists(const TableDefinition &table_definition) {
    const auto column_is_primary_key = [](const ColumnDefinition &column) {
      return column.primary_key;
    };
    const auto has_primary_keys =
        ranges::any_of(table_definition.columns, column_is_primary_key);

    const auto column_is_foreign_key = [](const ColumnDefinition &column) {
      return column.foreign_key.has_value();
    };
    const auto has_foreign_keys =
        ranges::any_of(table_definition.columns, column_is_foreign_key);

    const auto has_keys = has_primary_keys || has_foreign_keys;

    auto query = std::string{"CREATE TABLE IF NOT EXISTS \"" +
                             table_definition.table + "\"(\n"};

    for (const auto &column_def : table_definition.columns) {
      query += "\t\"" + column_def.column + "\" " +
               std::string{magic_enum::enum_name(column_def.data_type)} +
               " NOT NULL";

      if (column_def.unique) {
        query += " UNIQUE";
      }

      if (const auto query_not_done =
              (&column_def != &table_definition.columns.back()) || has_keys) {
        query += ",\n";
      }
    }

    if (has_primary_keys) {
      query += "PRIMARY KEY(";
      auto primary_key_columns = table_definition.columns |
                                 ranges::views::filter(column_is_primary_key);

      for (const auto &column_def : primary_key_columns) {
        query += "\"" + column_def.column + "\"";

        if (column_def.auto_increment) {
          query += " AUTOINCREMENT";
        }

        if (const auto query_not_done =
                &column_def != &primary_key_columns.back()) {
          query += ", ";
        }
      }

      query += ")";

      if (has_foreign_keys) {
        query += ",\n";
      }
    }

    if (has_foreign_keys) {
      auto foreign_key_columns = table_definition.columns |
                                 ranges::views::filter(column_is_foreign_key);

      for (const auto &column_def : foreign_key_columns) {
        const auto &foreign_key = *column_def.foreign_key;
        query += "FOREIGN KEY(\"" + column_def.column + "\") REFERENCES \"" +
                 foreign_key.table + "\"(\"" + foreign_key.column +
                 "\") ON DELETE CASCADE";

        if (const auto query_not_done =
                &column_def != &foreign_key_columns.back()) {
          query += ",\n";
        }
      }
    }

    query += "\n);";

    ExecuteQuery(*sqlite_db_, query);
  }

  void DropTable(const Table &table) {
    const auto query = std::string{"DROP TABLE \"" + table + "\";"};
    ExecuteQuery(*sqlite_db_, query);
  }

  void DeleteFrom(const Table &table, std::string_view where_clause) {
    const auto query = std::string{"DELETE FROM \"" + table + "\" " +
                                   std::string{where_clause}};
    ExecuteQuery(*sqlite_db_, query);
  }

  void UpdateRow(const Table &table, const Row &new_row_values,
                 std::string_view where_clause) {
    auto query = std::string{"UPDATE \"" + table + "\" SET "};
    const auto &cells = new_row_values.GetCells();

    for (const auto &cell : cells) {
      const auto &[column, value] = cell;

      query += "\"" + column + "\" = " + value.ToString();

      if (const auto query_not_done = &cell != &*cells.end()) {
        query += ", ";
      }
    }

    query += " " + std::string{where_clause};

    ExecuteQuery(*sqlite_db_, query);
  }

  [[nodiscard]] auto PrepareStatement(std::string_view query)
      -> std::unique_ptr<PreparedStatement> {
    auto sqlite_statement = PrepareSqliteStatement(*sqlite_db_, query);

    const auto &shared_sqlite_statement = prepared_statements_.emplace_back(
        sqlite_statement.get(),
        [](auto &sqlite_statement) { sqlite3_finalize(sqlite_statement); });

    Logger().debug("Prepared statement: {}", query);
    return std::make_unique<SqlitePreparedStatement>(
        shared_sqlite_statement,
        std::bind_front(&RemoveStatement, prepared_statements_));
  }

 private:
  gsl::not_null<sqlite3 *> sqlite_db_;
  std::vector<std::shared_ptr<sqlite3_stmt>> prepared_statements_{};
};

SqliteDb::SqliteDb(std::string_view file_name)
    : impl_{std::make_unique<Impl>(file_name)} {}

SqliteDb::~SqliteDb() = default;

void SqliteDb::CreateTableIfNotExists(const TableDefinition &table_definition) {
  impl_->CreateTableIfNotExists(table_definition);
}

void SqliteDb::DropTable(const Table &table) { impl_->DropTable(table); }

void SqliteDb::DeleteFrom(const Table &table, std::string_view where_clause) {
  impl_->DeleteFrom(table, where_clause);
}

void SqliteDb::UpdateRow(const Table &table, const Row &new_row_values,
                         std::string_view where_clause) {
  impl_->UpdateRow(table, new_row_values, where_clause);
}

[[nodiscard]] auto SqliteDb::PrepareStatement(std::string_view query)
    -> std::unique_ptr<PreparedStatement> {
  return impl_->PrepareStatement(query);
}

// /*
// ** This function is used to load the contents of a database file on disk
// ** into the "main" database of open database connection pInMemory, or
// ** to save the current contents of the database opened by pInMemory into
// ** a database file on disk. pInMemory is probably an in-memory database,
// ** but this function will also work fine if it is not.
// **
// ** Parameter zFilename points to a nul-terminated string containing the
// ** name of the database file on disk to load from or save to. If parameter
// ** isSave is non-zero, then the contents of the file zFilename are
// ** overwritten with the contents of the database opened by pInMemory. If
// ** parameter isSave is zero, then the contents of the database opened by
// ** pInMemory are replaced by data loaded from the file zFilename.
// **
// ** If the operation is successful, SQLITE_OK is returned. Otherwise, if
// ** an error occurs, an SQLite error code is returned.
// */
// int loadOrSaveDb(sqlite3 *pInMemory, const char *zFilename, int isSave) {
//   int rc;                  /* Function return code */
//   sqlite3 *pFile;          /* Database connection opened on zFilename */
//   sqlite3_backup *pBackup; /* Backup object used to copy data */
//   sqlite3 *pTo;            /* Database to copy to (pFile or pInMemory) */
//   sqlite3 *pFrom;          /* Database to copy from (pFile or pInMemory) */

//   /* Open the database file identified by zFilename. Exit early if this fails
//   ** for any reason. */
//   rc = sqlite3_open(zFilename, &pFile);
//   if (rc == SQLITE_OK) {
//     /* If this is a 'load' operation (isSave==0), then data is copied
//     ** from the database file just opened to database pInMemory.
//     ** Otherwise, if this is a 'save' operation (isSave==1), then data
//     ** is copied from pInMemory to pFile.  Set the variables pFrom and
//     ** pTo accordingly. */
//     pFrom = (isSave ? pInMemory : pFile);
//     pTo = (isSave ? pFile : pInMemory);

//     /* Set up the backup procedure to copy from the "main" database of
//     ** connection pFile to the main database of connection pInMemory.
//     ** If something goes wrong, pBackup will be set to NULL and an error
//     ** code and message left in connection pTo.
//     **
//     ** If the backup object is successfully created, call backup_step()
//     ** to copy data from pFile to pInMemory. Then call backup_finish()
//     ** to release resources associated with the pBackup object.  If an
//     ** error occurred, then an error code and message will be left in
//     ** connection pTo. If no error occurred, then the error code belonging
//     ** to pTo is set to SQLITE_OK.
//     */
//     pBackup = sqlite3_backup_init(pTo, "main", pFrom, "main");
//     if (pBackup) {
//       (void)sqlite3_backup_step(pBackup, -1);
//       (void)sqlite3_backup_finish(pBackup);
//     }
//     rc = sqlite3_errcode(pTo);
//   }

//   /* Close the database connection opened on database file zFilename
//   ** and return the result of this function. */
//   (void)sqlite3_close(pFile);
//   return rc;
// }

// /*
// ** Perform an online backup of database pDb to the database file named
// ** by zFilename. This function copies 5 database pages from pDb to
// ** zFilename, then unlocks pDb and sleeps for 250 ms, then repeats the
// ** process until the entire database is backed up.
// **
// ** The third argument passed to this function must be a pointer to a progress
// ** function. After each set of 5 pages is backed up, the progress function
// ** is invoked with two integer parameters: the number of pages left to
// ** copy, and the total number of pages in the source file. This information
// ** may be used, for example, to update a GUI progress bar.
// **
// ** While this function is running, another thread may use the database pDb,
// or
// ** another process may access the underlying database file via a separate
// ** connection.
// **
// ** If the backup process is successfully completed, SQLITE_OK is returned.
// ** Otherwise, if an error occurs, an SQLite error code is returned.
// */
// int backupDb(sqlite3 *pDb,               /* Database to back up */
//              const char *zFilename,      /* Name of file to back up to */
//              void (*xProgress)(int, int) /* Progress function to invoke */
// ) {
//   int rc;                  /* Function return code */
//   sqlite3 *pFile;          /* Database connection opened on zFilename */
//   sqlite3_backup *pBackup; /* Backup handle used to copy data */

//   /* Open the database file identified by zFilename. */
//   rc = sqlite3_open(zFilename, &pFile);
//   if (rc == SQLITE_OK) {
//     /* Open the sqlite3_backup object used to accomplish the transfer */
//     pBackup = sqlite3_backup_init(pFile, "main", pDb, "main");
//     if (pBackup) {
//       /* Each iteration of this loop copies 5 database pages from database
//       ** pDb to the backup database. If the return value of backup_step()
//       ** indicates that there are still further pages to copy, sleep for
//       ** 250 ms before repeating. */
//       do {
//         rc = sqlite3_backup_step(pBackup, 5);
//         xProgress(sqlite3_backup_remaining(pBackup),
//                   sqlite3_backup_pagecount(pBackup));
//         if (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED) {
//           sqlite3_sleep(250);
//         }
//       } while (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED);

//       /* Release resources allocated by backup_init(). */
//       (void)sqlite3_backup_finish(pBackup);
//     }
//     rc = sqlite3_errcode(pFile);
//   }

//   /* Close the database connection opened on database file zFilename
//   ** and return the result of this function. */
//   (void)sqlite3_close(pFile);
//   return rc;
// }

// // auto SqliteDb::Backup(std::string_view file_name) const -> bool
// // {
// //   auto sqlite_db = (sqlite3 *){nullptr};
// //   auto result_code = sqlite3_open(file_name.data(), &sqlite_db);
// //   ABSL_ASSERT(sqlite_db != nullptr);

// //   auto sqlite_db = SqliteDb{sqlite_db};

// //   if (result_code != SQLITE_OK) {
// //     spdlog::error("Cannot open DB from {}: {}", file_name,
// //                   sqlite3_errmsg(sqlite_db.sqlite_db_));
// //     return false;
// //   }

// //   result_code =
// //       sqlite3_exec(sqlite_db.sqlite_db_, "PRAGMA foreign_keys = ON",
// //                    nullptr, nullptr, nullptr);

// //   if (result_code != SQLITE_OK) {
// //     spdlog::error("Failed to enable foreign key checks in SQLite DB");
// //   }

// //   const auto full_file_name = GetDbFileName(*sqlite_db.sqlite_db_);
// //   ABSL_ASSERT(full_file_name.has_value());

// //   spdlog::info("Opened DB from {}", *full_file_name);
// //   return std::optional<SqliteDb>{std::move(sqlite_db)};
// // }
}  // namespace stonks::db::sqlite