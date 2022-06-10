#include "sqlite_db.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <sqlite3.h>

#include <functional>
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

#include "db_enum_conversions.h"  // IWYU pragma: keep
#include "db_prepared_statement.h"
#include "sqlite_prepared_statement.h"
#include "sqlite_utils.h"

namespace stonks::db::sqlite {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = spdlog::stdout_color_mt("SqliteDb");
  return *logger;
}

[[nodiscard]] auto GetDbFileName(sqlite3 &sqlite_db) -> std::string {
  const auto *const file_name = sqlite3_db_filename(&sqlite_db, nullptr);

  if (file_name == nullptr) {
    return {};
  }

  return file_name;
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
    throw std::runtime_error{"Cannot close DB from " + file_name};
  }

  Logger().info("Closed DB from {}", file_name);
}
}  // namespace

class SqliteDb::Impl {
 public:
  explicit Impl(gsl::not_null<sqlite3 *> sqlite_db) : sqlite_db_{sqlite_db} {}

  Impl(const Impl &) = delete;
  Impl(Impl &&) = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) -> Impl & = default;

  ~Impl() try {
    prepared_statements_.clear();
    CloseSqliteDb(*sqlite_db_);
  } catch (const std::exception &exception) {
    Logger().error(exception.what());
  }

  [[nodiscard]] auto PrepareStatement(std::string_view query)
      -> std::unique_ptr<PreparedStatement> {
    auto sqlite_statement = PrepareSqliteStatement(*sqlite_db_, query);

    const auto &shared_sqlite_statement = prepared_statements_.emplace_back(
        sqlite_statement.get(),
        [](auto &sqlite_statement) { sqlite3_finalize(sqlite_statement); });

    Logger().info("Prepared statement: {}", query);
    return std::make_unique<SqlitePreparedStatement>(
        shared_sqlite_statement,
        std::bind_front(&RemoveStatement, prepared_statements_));
  }

  void WriteToFile(std::string_view file_path) {
    utils::WriteSqliteDbToFile(*sqlite_db_, file_path);
  }

 private:
  gsl::not_null<sqlite3 *> sqlite_db_;
  std::vector<std::shared_ptr<sqlite3_stmt>> prepared_statements_{};
};

SqliteDb::SqliteDb(gsl::not_null<sqlite3 *> sqlite_db)
    : impl_{std::make_unique<Impl>(sqlite_db)} {}

SqliteDb::~SqliteDb() = default;

auto SqliteDb::PrepareStatement(std::string_view query)
    -> std::unique_ptr<PreparedStatement> {
  return impl_->PrepareStatement(query);
}

void SqliteDb::WriteToFile(std::string_view file_path) {
  impl_->WriteToFile(file_path);
}
}  // namespace stonks::db::sqlite