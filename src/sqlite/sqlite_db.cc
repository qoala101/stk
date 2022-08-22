#include "sqlite_db.h"

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <sqlite3.h>

#include <memory>
#include <utility>

#include "cpp_not_null.h"
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
        sqlite_db_facade_{cpp::AssumeNn(sqlite_db_handle_.get())} {}

  Impl(const Impl &) = delete;
  Impl(Impl &&) noexcept = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) noexcept -> Impl & = default;

  ~Impl() noexcept = default;

  [[nodiscard]] auto PrepareStatement(
      const std::string &query, const sqldb::RowDefinition &result_definition)
      -> cpp::NnUp<sqldb::ISelectStatement> {
    return cpp::MakeNnUp<SelectStatement>(
        PreparedStatementHandle{
            sqlite_db_handle_,
            sqlite_db_facade_.CreatePreparedStatement(query)},
        result_definition);
  }

  [[nodiscard]] auto PrepareStatement(const std::string &query)
      -> cpp::NnUp<sqldb::IUpdateStatement> {
    return cpp::MakeNnUp<UpdateStatement>(PreparedStatementHandle{
        sqlite_db_handle_, sqlite_db_facade_.CreatePreparedStatement(query)});
  }

  void WriteToFile(std::string_view file_path) {
    sqlite_db_facade_.WriteToFile(file_path);
  }

 private:
  cpp::NnSp<sqlite3> sqlite_db_handle_;
  DbFacade sqlite_db_facade_;
};

Db::Db(SqliteDbHandle sqlite_db_handle)
    : impl_{cpp::MakeNnUp<Impl>(std::move(sqlite_db_handle))} {}

Db::~Db() noexcept = default;

auto Db::PrepareStatement(std::string query,
                          sqldb::RowDefinition result_definition)
    -> cpp::NnUp<sqldb::ISelectStatement> {
  return impl_->PrepareStatement(query, result_definition);
}

auto Db::PrepareStatement(std::string query)
    -> cpp::NnUp<sqldb::IUpdateStatement> {
  return impl_->PrepareStatement(query);
}

void Db::WriteToFile(std::string_view file_path) {
  impl_->WriteToFile(file_path);
}
}  // namespace stonks::sqlite