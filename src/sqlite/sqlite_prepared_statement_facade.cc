#include "sqlite_prepared_statement_facade.h"

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <sqlite3.h>

#include <cstdint>
#include <gsl/assert>
#include <gsl/util>
#include <memory>
#include <string>

#include "cpp_message_exception.h"
#include "cpp_not_null.h"

namespace stonks::sqlite {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger =
      spdlog::stdout_color_mt("sqlite::PreparedStatementFacade");
  return *logger;
}

void BindParam(sqlite3_stmt &statement, int index, const sqldb::Value &value) {
  Expects(index > 0);

  if (value.IsNull()) {
    return;
  }

  auto result_code = SQLITE_OK;

  switch (value.GetType()) {
    case sqldb::DataType::kBool:
      result_code = sqlite3_bind_int(&statement, index,
                                     static_cast<int>(value.GetBool()));
      break;
    case sqldb::DataType::kInt:
      result_code = sqlite3_bind_int(&statement, index, value.GetInt());
      break;
    case sqldb::DataType::kInt64:
      result_code = sqlite3_bind_int64(&statement, index, value.GetInt64());
      break;
    case sqldb::DataType::kDouble:
      result_code = sqlite3_bind_double(&statement, index, value.GetDouble());
      break;
    case sqldb::DataType::kString: {
      const auto &string = value.GetString();
      result_code =
          sqlite3_bind_text(&statement, index, string.c_str(),
                            gsl::narrow_cast<int>(string.length()), nullptr);
      break;
    }
  }

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{
        "Couldn't bind parameter to prepared statement: " +
        std::to_string(result_code)};
  }
}

[[nodiscard]] auto GetValue(sqlite3_stmt &statement, int index,
                            sqldb::DataType type) -> sqldb::Value {
  Expects(index >= 0);

  if (sqlite3_column_type(&statement, index) == SQLITE_NULL) {
    return {};
  }

  switch (type) {
    case sqldb::DataType::kBool:
      return sqldb::Value{
          static_cast<bool>(sqlite3_column_int(&statement, index))};
    case sqldb::DataType::kInt:
      return sqldb::Value{sqlite3_column_int(&statement, index)};
    case sqldb::DataType::kInt64:
      return sqldb::Value{int64_t{sqlite3_column_int64(&statement, index)}};
    case sqldb::DataType::kDouble:
      return sqldb::Value{sqlite3_column_double(&statement, index)};
    case sqldb::DataType::kString:
      // NOLINTNEXTLINE(*-reinterpret-cast)
      return sqldb::Value{std::string{reinterpret_cast<const char *>(
          sqlite3_column_text(&statement, index))}};
  }

  Expects(false);
}
}  // namespace

PreparedStatementFacade::PreparedStatementFacade(
    cpp::Nn<sqlite3_stmt *> sqlite_statement)
    : sqlite_statement_{sqlite_statement.as_nullable()} {
  Ensures(sqlite_statement_ != nullptr);
}

void PreparedStatementFacade::Reset() {
  Expects(sqlite_statement_ != nullptr);

  const auto result_code = sqlite3_reset(sqlite_statement_);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{"Couldn't reset prepared statement: " +
                                std::to_string(result_code)};
  }
}

void PreparedStatementFacade::BindParams(
    const std::vector<sqldb::Value> &params) {
  Expects(sqlite_statement_ != nullptr);

  for (auto i = 0; i < params.size(); ++i) {
    BindParam(*sqlite_statement_, i + 1, params[i]);
  }
}

auto PreparedStatementFacade::Step() -> ResultCode {
  Expects(sqlite_statement_ != nullptr);
  return {sqlite3_step(sqlite_statement_)};
}

auto PreparedStatementFacade::GetStepValues(
    const std::vector<sqldb::DataType> &value_types) const
    -> std::vector<sqldb::Value> {
  Expects(sqlite_statement_ != nullptr);

  const auto num_values = value_types.size();

  auto values = std::vector<sqldb::Value>{};
  values.reserve(num_values);

  for (auto i = 0; i < num_values; ++i) {
    values.emplace_back(GetValue(*sqlite_statement_, i, value_types[i]));
  }

  Ensures(values.size() == value_types.size());
  return values;
}

void PreparedStatementFacade::Finalize() {
  Expects(sqlite_statement_ != nullptr);

  const auto result_code = sqlite3_finalize(sqlite_statement_);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{"Couldn't finalize prepared statement: " +
                                std::to_string(result_code)};
  }

  sqlite_statement_ = nullptr;
  Ensures(sqlite_statement_ == nullptr);
}
}  // namespace stonks::sqlite