#include "sqlite_native_db_facade.h"

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
#include "sqlite_native_db_handles_factory.h"
#include "sqlite_native_handles.h"

namespace stonks::sqlite {
namespace {
using NullableNativeStatementHandle = std::remove_cvref_t<
    decltype(std::declval<NativeStatementHandle>().as_nullable())>;

auto GetAssociatedFileName [[nodiscard]] (sqlite3 &native_db) -> std::string {
  const auto *const file_name = sqlite3_db_filename(&native_db, nullptr);

  if (file_name == nullptr) {
    return {};
  }

  return file_name;
}

auto GetPrintableFileName [[nodiscard]] (sqlite3 &native_db) -> std::string {
  auto file_name = GetAssociatedFileName(native_db);

  if (file_name.empty()) {
    return "memory";
  }

  return file_name;
}
}  // namespace

NativeDbFacade::NativeDbFacade(di::Factory<log::ILogger> logger_factory,
                   cpp::Nn<sqlite3 *> native_db)
    : logger_factory_{std::move(logger_factory)},
      logger_{logger_factory_.Create()},
      native_db_{native_db.as_nullable()},
      handles_factory_{logger_factory_} {
  Ensures(native_db_ != nullptr);
}

void NativeDbFacade::WriteToFile(const FilePath &file_path) const {
  Expects(native_db_ != nullptr);

  auto file_db_handle = handles_factory_.CreateHandleToFileDb(file_path);
  NativeDbFacade{logger_factory_, cpp::AssumeNn(file_db_handle.get())}.CopyDataFrom(
      *native_db_);

  logger_->LogImportantEvent(fmt::format("Stored DB to {}", *file_path));
}

void NativeDbFacade::CopyDataFrom(sqlite3 &other_db) const {
  Expects(native_db_ != nullptr);

  auto *backup = sqlite3_backup_init(native_db_, "main", &other_db, "main");
  sqlite3_backup_step(backup, -1);
  const auto result_code = sqlite3_backup_finish(backup);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{
        fmt::format("Couldn't copy data from other DB: {}", result_code)};
  }
}

auto NativeDbFacade::CreatePreparedStatement(const sqldb::Query &query) const
    -> NativeStatementHandle {
  Expects(native_db_ != nullptr);

  auto *native_statement = static_cast<sqlite3_stmt *>(nullptr);
  const auto result_code = sqlite3_prepare_v3(
      native_db_, query->c_str(), gsl::narrow_cast<int>(query->length()) + 1,
      SQLITE_PREPARE_PERSISTENT, &native_statement, nullptr);

  if (native_statement == nullptr) {
    throw cpp::MessageException{
        fmt::format("Couldn't prepare the statement for query {}: {}", *query,
                    result_code)};
  }

  logger_->LogImportantEvent(
      fmt::format("Prepared statement for query {}", *query));

  return NativeStatementHandle{cpp::AssumeNn(NullableNativeStatementHandle{
      native_statement,
      detail::NativeStatementFinalizer{logger_factory_.Create()}})};
}

void NativeDbFacade::EnableForeignKeys() const { SetPragma("foreign_keys", "ON"); }

void NativeDbFacade::TurnOffSynchronization() const { SetPragma("synchronous", "OFF"); }

void NativeDbFacade::Close() {
  Expects(native_db_ != nullptr);

  const auto file_name = GetPrintableFileName(*native_db_);
  const auto result_code = sqlite3_close(native_db_);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{
        fmt::format("Couldn't close DB from {}", file_name)};
  }

  logger_->LogImportantEvent(fmt::format("Closed DB from {}", file_name));

  native_db_ = nullptr;
  Ensures(native_db_ == nullptr);
}

void NativeDbFacade::SetPragma(std::string_view pragma,
                         std::string_view value) const {
  Expects(native_db_ != nullptr);

  const auto query = fmt::format("PRAGMA {} = {}", pragma, value);
  const auto result_code =
      sqlite3_exec(native_db_, query.c_str(), nullptr, nullptr, nullptr);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{fmt::format("Couldn't set {} pragma to {}: {}",
                                            pragma, value, result_code)};
  }
}
}  // namespace stonks::sqlite