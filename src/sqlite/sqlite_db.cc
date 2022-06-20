#include "sqlite_db.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <sqlite3.h>

#include <functional>
#include <gsl/assert>
#include <gsl/pointers>
#include <memory>
#include <range/v3/action/erase.hpp>
#include <range/v3/algorithm/remove_if.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/range/dangling.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "sqldb_enum_conversions.h"  // IWYU pragma: keep
#include "sqldb_select_statement.h"
#include "sqldb_update_statement.h"
#include "sqlite_prepared_statement_handle.h"
#include "sqlite_select_statement.h"
#include "sqlite_update_statement.h"
#include "sqlite_utils.h"

namespace stonks::sqlite {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("SqliteDb");
  return *logger;
}
}  // namespace

class SqliteDb::Impl {
 public:
  explicit Impl(SqliteDbHandle &&sqlite_db_handle)
      : sqlite_db_handle_{std::move(sqlite_db_handle)} {
    Expects(sqlite_db_handle_ != nullptr);
  }

  Impl(const Impl &) = delete;
  Impl(Impl &&) noexcept = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) noexcept -> Impl & = default;

  ~Impl() = default;

  [[nodiscard]] auto PrepareStatement(
      std::string_view query, const sqldb::RowDefinition &result_definition)
      -> std::unique_ptr<sqldb::ISelectStatement> {
    auto statement = std::make_unique<SqliteSelectStatement>(
        SqlitePreparedStatementHandle{
            CreateSqliteStatement(query),
            std::bind_front(&Impl::RemoveSqliteStatement, this)},
        result_definition);
    Ensures(statement != nullptr);
    return statement;
  }

  [[nodiscard]] auto PrepareStatement(std::string_view query)
      -> std::unique_ptr<sqldb::IUpdateStatement> {
    auto statement =
        std::make_unique<SqliteUpdateStatement>(SqlitePreparedStatementHandle{
            CreateSqliteStatement(query),
            std::bind_front(&Impl::RemoveSqliteStatement, this)});
    Ensures(statement != nullptr);
    return statement;
  }

  void WriteToFile(std::string_view file_path) {
    Expects(sqlite_db_handle_ != nullptr);
    WriteSqliteDbToFile(*sqlite_db_handle_, file_path);
  }

 private:
  [[nodiscard]] auto CreateSqliteStatement(std::string_view query)
      -> const std::shared_ptr<sqlite3_stmt> & {
    Expects(sqlite_db_handle_ != nullptr);

    auto *sqlite_statement = (sqlite3_stmt *){};
    sqlite3_prepare_v3(sqlite_db_handle_.get(), query.data(),
                       static_cast<int>(query.length()) + 1,
                       SQLITE_PREPARE_PERSISTENT, &sqlite_statement, nullptr);

    if (sqlite_statement == nullptr) {
      throw std::runtime_error{
          std::string{"Couldn't prepare statement for query: "} + query.data()};
    }

    const auto &statement = prepared_statements_.emplace_back(
        sqlite_statement,
        [](auto &sqlite_statement) { sqlite3_finalize(sqlite_statement); });
    Ensures(statement != nullptr);
    return statement;
  }

  void RemoveSqliteStatement(sqlite3_stmt &sqlite_statement) {
    ranges::erase(
        prepared_statements_,
        ranges::remove_if(prepared_statements_,
                          [&sqlite_statement](const auto &prepared_statement) {
                            return &(*prepared_statement) == &sqlite_statement;
                          }),
        prepared_statements_.end());
  }

  SqliteDbHandle sqlite_db_handle_{};
  std::vector<std::shared_ptr<sqlite3_stmt>> prepared_statements_{};
};

SqliteDb::SqliteDb(SqliteDbHandle &&sqlite_db_handle)
    : impl_{std::make_unique<Impl>(std::move(sqlite_db_handle))} {}

SqliteDb::~SqliteDb() = default;

auto SqliteDb::PrepareStatement(std::string_view query,
                                const sqldb::RowDefinition &result_definition)
    -> std::unique_ptr<sqldb::ISelectStatement> {
  return impl_->PrepareStatement(query, result_definition);
}

auto SqliteDb::PrepareStatement(std::string_view query)
    -> std::unique_ptr<sqldb::IUpdateStatement> {
  return impl_->PrepareStatement(query);
}

void SqliteDb::WriteToFile(std::string_view file_path) {
  impl_->WriteToFile(file_path);
}
}  // namespace stonks::sqlite