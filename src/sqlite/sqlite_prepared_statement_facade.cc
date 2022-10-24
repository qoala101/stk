#include "sqlite_prepared_statement_facade.h"

#include <fmt/core.h>
#include <sqlite3.h>

#include <cstdint>
#include <gsl/assert>
#include <gsl/util>
#include <string>
#include <variant>

#include "cpp_message_exception.h"
#include "cpp_not_null.h"
#include "sqldb_data_type.h"

namespace stonks::sqlite {
namespace {
void BindParam(sqlite3_stmt &statement, int index, const sqldb::Value &value) {
  Expects(index > 0);

  if (value.IsNull()) {
    return;
  }

  // auto result_code = SQLITE_OK;

  auto result_code = std::visit(
      [&statement, index, &value](const auto &v) {
        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, sqldb::DataType<bool>>) {
          return sqlite3_bind_int(&statement, index,
                                  static_cast<int>(value.Get<bool>()));
        } else if constexpr (cpp::DecaysTo<V, sqldb::DataType<int>>) {
          return sqlite3_bind_int(&statement, index, value.Get<int>());
        } else if constexpr (cpp::DecaysTo<V, sqldb::DataType<int64_t>>) {
          return sqlite3_bind_int64(&statement, index, value.Get<int64_t>());
        } else if constexpr (cpp::DecaysTo<V, sqldb::DataType<double>>) {
          return sqlite3_bind_double(&statement, index, value.Get<double>());
        } else if constexpr (cpp::DecaysTo<V, sqldb::DataType<std::string>>) {
          const auto &string = value.Get<std::string>();
          return sqlite3_bind_text(&statement, index, string.c_str(),
                                   gsl::narrow_cast<int>(string.length()),
                                   nullptr);
        } else {
          Expects(false);
        }
      },
      value.GetType().value);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{fmt::format(
        "Couldn't bind parameter to prepared statement: {}", result_code)};
  }
}

[[nodiscard]] auto GetValue(sqlite3_stmt &statement, int index,
                            sqldb::DataTypeVariant type) {
  Expects(index >= 0);

  if (sqlite3_column_type(&statement, index) == SQLITE_NULL) {
    return sqldb::Value{};
  }

  return std::visit(
      [&statement, index](const auto &v) {
        using V = decltype(v);

        if constexpr (cpp::DecaysTo<V, sqldb::DataType<bool>>) {
          return sqldb::Value{
              static_cast<bool>(sqlite3_column_int(&statement, index))};
        } else if constexpr (cpp::DecaysTo<V, sqldb::DataType<int>>) {
          return sqldb::Value{sqlite3_column_int(&statement, index)};
        } else if constexpr (cpp::DecaysTo<V, sqldb::DataType<int64_t>>) {
          return sqldb::Value{int64_t{sqlite3_column_int64(&statement, index)}};
        } else if constexpr (cpp::DecaysTo<V, sqldb::DataType<double>>) {
          return sqldb::Value{sqlite3_column_double(&statement, index)};
        } else if constexpr (cpp::DecaysTo<V, sqldb::DataType<std::string>>) {
          // NOLINTNEXTLINE(*-reinterpret-cast)
          return sqldb::Value{std::string{reinterpret_cast<const char *>(
              sqlite3_column_text(&statement, index))}};
        } else {
          Expects(false);
        }
      },
      type.value);

  Expects(false);
}
}  // namespace

PreparedStatementFacade::PreparedStatementFacade(
    cpp::Nn<sqlite3_stmt *> sqlite_statement)
    : sqlite_statement_{sqlite_statement.as_nullable()} {
  Ensures(sqlite_statement_ != nullptr);
}

void PreparedStatementFacade::Reset() const {
  Expects(sqlite_statement_ != nullptr);

  const auto result_code = sqlite3_reset(sqlite_statement_);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{
        fmt::format("Couldn't reset prepared statement: {}", result_code)};
  }
}

void PreparedStatementFacade::BindParams(
    const std::vector<sqldb::Value> &params) const {
  Expects(sqlite_statement_ != nullptr);

  for (auto i = 0; i < gsl::narrow_cast<int>(params.size()); ++i) {
    BindParam(*sqlite_statement_, i + 1, params[i]);
  }
}

auto PreparedStatementFacade::Step() const -> ResultCode {
  Expects(sqlite_statement_ != nullptr);
  return {sqlite3_step(sqlite_statement_)};
}

auto PreparedStatementFacade::GetStepValues(
    const std::vector<sqldb::DataTypeVariant> &value_types) const
    -> std::vector<sqldb::Value> {
  Expects(sqlite_statement_ != nullptr);

  const auto num_values = value_types.size();

  auto values = std::vector<sqldb::Value>{};
  values.reserve(num_values);

  for (auto i = 0; i < gsl::narrow_cast<int>(num_values); ++i) {
    values.emplace_back(GetValue(*sqlite_statement_, i, value_types[i]));
  }

  Ensures(values.size() == value_types.size());
  return values;
}

void PreparedStatementFacade::Finalize() {
  Expects(sqlite_statement_ != nullptr);

  const auto result_code = sqlite3_finalize(sqlite_statement_);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{
        fmt::format("Couldn't finalize prepared statement: {}", result_code)};
  }

  sqlite_statement_ = nullptr;
  Ensures(sqlite_statement_ == nullptr);
}
}  // namespace stonks::sqlite