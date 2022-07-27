#include "sqlite_db.h"

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <sqlite3.h>

#include <memory>
#include <utility>

#include "not_null.hpp"
#include "sqldb_enums_to_string.h"  // IWYU pragma: keep
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqlite_db_facade.h"
#include "sqlite_prepared_statement_handle.h"
#include "sqlite_select_statement.h"
#include "sqlite_types.h"
#include "sqlite_update_statement.h"

namespace stonks::sqlite {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("sqlite::Db");
  return *logger;
}
}  // namespace

class Db::Impl {
 public:
  explicit Impl(SqliteDbHandle sqlite_db_handle)
      : sqlite_db_handle_{std::move(sqlite_db_handle)},
        sqlite_db_facade_{cpp::assume_not_null(sqlite_db_handle_.get())} {}

  Impl(const Impl &) = delete;
  Impl(Impl &&) noexcept = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) noexcept -> Impl & = default;

  ~Impl() noexcept = default;

  [[nodiscard]] auto PrepareStatement(
      const std::string &query, const sqldb::RowDefinition &result_definition)
      -> cpp::not_null<std::unique_ptr<sqldb::ISelectStatement>> {
    return cpp::assume_not_null(std::make_unique<SelectStatement>(
        PreparedStatementHandle{
            sqlite_db_handle_,
            sqlite_db_facade_.CreatePreparedStatement(query)},
        result_definition));
  }

  [[nodiscard]] auto PrepareStatement(const std::string &query)
      -> cpp::not_null<std::unique_ptr<sqldb::IUpdateStatement>> {
    return cpp::assume_not_null(
        std::make_unique<UpdateStatement>(PreparedStatementHandle{
            sqlite_db_handle_,
            sqlite_db_facade_.CreatePreparedStatement(query)}));
  }

  void WriteToFile(std::string_view file_path) {
    sqlite_db_facade_.WriteToFile(file_path);
  }

 private:
  cpp::not_null<std::shared_ptr<sqlite3>> sqlite_db_handle_;
  DbFacade sqlite_db_facade_;
};

Db::Db(SqliteDbHandle sqlite_db_handle)
    : impl_{cpp::assume_not_null(
          std::make_unique<Impl>(std::move(sqlite_db_handle)))} {}

Db::~Db() noexcept = default;

auto Db::PrepareStatement(std::string query,
                          sqldb::RowDefinition result_definition)
    -> cpp::not_null<std::unique_ptr<sqldb::ISelectStatement>> {
  return impl_->PrepareStatement(query, result_definition);
}

auto Db::PrepareStatement(std::string query)
    -> cpp::not_null<std::unique_ptr<sqldb::IUpdateStatement>> {
  return impl_->PrepareStatement(query);
}

void Db::WriteToFile(std::string_view file_path) {
  impl_->WriteToFile(file_path);
}
}  // namespace stonks::sqlite