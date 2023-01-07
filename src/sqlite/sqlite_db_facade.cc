#include "sqlite_db_facade.h"

#include <fmt/core.h>
#include <sqlite3.h>

#include <gsl/assert>
#include <gsl/util>
#include <memory>
#include <not_null.hpp>
#include <string>
#include <type_traits>
#include <utility>

#include "cpp_message_exception.h"
#include "cpp_not_null.h"
#include "cpp_typed_struct.h"
#include "sqlite_db_handles_factory.h"
#include "sqlite_raw_handles.h"

namespace stonks::sqlite {
namespace {
using NullableSqliteStatementHandle = std::remove_cvref_t<
    decltype(std::declval<SqliteStatementHandle>().as_nullable())>;

auto GetAssociatedFileName [[nodiscard]] (sqlite3 &sqlite_db) -> std::string {
  const auto *const file_name = sqlite3_db_filename(&sqlite_db, nullptr);

  if (file_name == nullptr) {
    return {};
  }

  return file_name;
}

auto GetPrintableFileName [[nodiscard]] (sqlite3 &sqlite_db) -> std::string {
  auto file_name = GetAssociatedFileName(sqlite_db);

  if (file_name.empty()) {
    return "memory";
  }

  return file_name;
}
}  // namespace

DbFacade::DbFacade(di::Factory<log::ILogger> logger_factory,
                   cpp::Nn<sqlite3 *> sqlite_db)
    : logger_factory_{std::move(logger_factory)},
      logger_{logger_factory_.Create()},
      sqlite_db_{sqlite_db.as_nullable()},
      handles_factory_{logger_factory_} {
  Ensures(sqlite_db_ != nullptr);
}

void DbFacade::WriteToFile(const FilePath &file_path) const {
  Expects(sqlite_db_ != nullptr);

  auto file_db_handle = handles_factory_.CreateHandleToFileDb(file_path);
  DbFacade{logger_factory_, cpp::AssumeNn(file_db_handle.get())}.CopyDataFrom(
      *sqlite_db_);

  logger_->LogImportantEvent(fmt::format("Stored DB to {}", *file_path));
}

void DbFacade::CopyDataFrom(sqlite3 &other_db) const {
  Expects(sqlite_db_ != nullptr);

  auto *backup = sqlite3_backup_init(sqlite_db_, "main", &other_db, "main");
  sqlite3_backup_step(backup, -1);
  const auto result_code = sqlite3_backup_finish(backup);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{
        fmt::format("Couldn't copy data from other DB: {}", result_code)};
  }
}

auto DbFacade::CreatePreparedStatement(const sqldb::Query &query) const
    -> SqliteStatementHandle {
  Expects(sqlite_db_ != nullptr);

  auto *sqlite_statement = static_cast<sqlite3_stmt *>(nullptr);
  const auto result_code = sqlite3_prepare_v3(
      sqlite_db_, query->c_str(), gsl::narrow_cast<int>(query->length()) + 1,
      SQLITE_PREPARE_PERSISTENT, &sqlite_statement, nullptr);

  if (sqlite_statement == nullptr) {
    throw cpp::MessageException{
        fmt::format("Couldn't prepare the statement for query {}: {}", *query,
                    result_code)};
  }

  logger_->LogImportantEvent(
      fmt::format("Prepared statement for query {}", *query));

  return SqliteStatementHandle{cpp::AssumeNn(NullableSqliteStatementHandle{
      sqlite_statement,
      detail::SqliteStatementFinalizer{logger_factory_.Create()}})};
}

void DbFacade::EnableForeignKeys() const { SetPragma("foreign_keys", "ON"); }

void DbFacade::TurnOffSynchronization() const { SetPragma("synchronous", "OFF"); }

void DbFacade::Close() {
  Expects(sqlite_db_ != nullptr);

  const auto file_name = GetPrintableFileName(*sqlite_db_);
  const auto result_code = sqlite3_close(sqlite_db_);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{
        fmt::format("Couldn't close DB from {}", file_name)};
  }

  logger_->LogImportantEvent(fmt::format("Closed DB from {}", file_name));

  sqlite_db_ = nullptr;
  Ensures(sqlite_db_ == nullptr);
}

void DbFacade::SetPragma(std::string_view pragma,
                         std::string_view value) const {
  Expects(sqlite_db_ != nullptr);

  const auto query = fmt::format("PRAGMA {} = {}", pragma, value);
  const auto result_code =
      sqlite3_exec(sqlite_db_, query.c_str(), nullptr, nullptr, nullptr);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{fmt::format("Couldn't set {} pragma to {}: {}",
                                            pragma, value, result_code)};
  }
}
}  // namespace stonks::sqlite