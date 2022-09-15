#include "sqlite_db_facade.h"

#include <sqlite3.h>

#include <gsl/assert>
#include <gsl/util>
#include <memory>
#include <string>
#include <utility>

#include "cpp_format.h"
#include "cpp_message_exception.h"
#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"
#include "not_null.hpp"
#include "sqlite_db_handles_factory.h"
#include "sqlite_raw_handles.h"

namespace stonks::sqlite {
namespace {
using NullableSqliteStatementHandle =
    std::decay_t<decltype(std::declval<SqliteStatementHandle>().as_nullable())>;

[[nodiscard]] auto GetAssociatedFileName(sqlite3 &sqlite_db) -> std::string {
  const auto *const file_name = sqlite3_db_filename(&sqlite_db, nullptr);

  if (file_name == nullptr) {
    return {};
  }

  return file_name;
}

[[nodiscard]] auto GetPrintableFileName(sqlite3 &sqlite_db) -> std::string {
  auto file_name = GetAssociatedFileName(sqlite_db);

  if (file_name.empty()) {
    return "memory";
  }

  return file_name;
}
}  // namespace

DbFacade::DbFacade(cpp::NnSp<log::ILogger> logger, cpp::Nn<sqlite3 *> sqlite_db)
    : logger_{std::move(logger)},
      sqlite_db_{sqlite_db.as_nullable()},
      handles_factory_{logger_} {
  Ensures(sqlite_db_ != nullptr);
}

void DbFacade::WriteToFile(const FilePath &file_path) const {
  Expects(sqlite_db_ != nullptr);

  auto file_db_handle = handles_factory_.CreateHandleToFileDb(file_path);
  DbFacade{logger_, cpp::AssumeNn(file_db_handle.get())}.CopyDataFrom(
      *sqlite_db_);

  logger_->LogImportantEvent(cpp::Format("Stored DB to {}", file_path.value));
}

void DbFacade::CopyDataFrom(sqlite3 &other_db) const {
  Expects(sqlite_db_ != nullptr);

  auto *backup = sqlite3_backup_init(sqlite_db_, "main", &other_db, "main");
  sqlite3_backup_step(backup, -1);
  const auto result_code = sqlite3_backup_finish(backup);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{
        cpp::Format("Couldn't copy data from other DB: {}", result_code)};
  }
}

auto DbFacade::CreatePreparedStatement(const sqldb::Query &query) const
    -> SqliteStatementHandle {
  Expects(sqlite_db_ != nullptr);

  auto *sqlite_statement = (sqlite3_stmt *){};
  sqlite3_prepare_v3(sqlite_db_, query.value.c_str(),
                     gsl::narrow_cast<int>(query.value.length()) + 1,
                     SQLITE_PREPARE_PERSISTENT, &sqlite_statement, nullptr);

  if (sqlite_statement == nullptr) {
    throw cpp::MessageException{cpp::Format(
        "Couldn't prepare the statement for query: {}", query.value)};
  }

  logger_->LogImportantEvent(
      cpp::Format("Prepared statement for query: {}", query.value));

  return SqliteStatementHandle{cpp::AssumeNn(NullableSqliteStatementHandle{
      sqlite_statement, detail::SqliteStatementFinalizer{logger_}})};
}

void DbFacade::EnableForeignKeys() const {
  Expects(sqlite_db_ != nullptr);

  const auto result_code = sqlite3_exec(sqlite_db_, "PRAGMA foreign_keys = ON",
                                        nullptr, nullptr, nullptr);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{cpp::Format(
        "Couldn't set foreign_keys pragma on new DB: {}", result_code)};
  }
}

void DbFacade::Close() {
  Expects(sqlite_db_ != nullptr);

  const auto file_name = GetPrintableFileName(*sqlite_db_);
  const auto result_code = sqlite3_close(sqlite_db_);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{
        cpp::Format("Couldn't close DB from {}", file_name)};
  }

  logger_->LogImportantEvent(cpp::Format("Closed DB from {}", file_name));

  sqlite_db_ = nullptr;
  Ensures(sqlite_db_ == nullptr);
}
}  // namespace stonks::sqlite