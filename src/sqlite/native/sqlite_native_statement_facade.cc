#include "sqlite_native_statement_facade.h"

#include <fmt/core.h>
#include <sqlite3.h>

#include <cstdint>
#include <gsl/assert>
#include <gsl/util>
#include <string>
#include <type_traits>
#include <variant>

#include "cpp_message_exception.h"
#include "cpp_typed_struct.h"
#include "sqldb_types.h"

namespace stonks::sqlite {
namespace {
void BindParam(sqlite3_stmt &statement, int index, const sqldb::Value &value) {
  Expects(index > 0);

  if (value.IsNull()) {
    return;
  }

  auto result_code = std::visit(
      [&statement, index, &value](auto v) {
        using V = typename decltype(v)::Type;

        if constexpr (std::is_same_v<V, bool>) {
          return sqlite3_bind_int(&statement, index, int{value.Get<V>()});
        } else if constexpr (std::is_same_v<V, int>) {
          return sqlite3_bind_int(&statement, index, value.Get<V>());
        } else if constexpr (std::is_same_v<V, int64_t>) {
          return sqlite3_bind_int64(&statement, index, value.Get<V>());
        } else if constexpr (std::is_same_v<V, double>) {
          return sqlite3_bind_double(&statement, index, value.Get<V>());
        } else if constexpr (std::is_same_v<V, std::string>) {
          const auto &string = value.Get<V>();
          return sqlite3_bind_text(&statement, index, string.c_str(),
                                   gsl::narrow_cast<int>(string.length()),
                                   nullptr);
        } else {
          Expects(false);
        }
      },
      *value.GetType());

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{fmt::format(
        "Couldn't bind parameter to prepared statement: {}", result_code)};
  }
}

auto GetValue [[nodiscard]] (sqlite3_stmt &statement, int index,
                             const sqldb::DataTypeVariant &type) {
  Expects(index >= 0);

  if (sqlite3_column_type(&statement, index) == SQLITE_NULL) {
    return sqldb::Value{};
  }

  return std::visit(
      [&statement, index](const auto &v) {
        using V = typename std::remove_cvref_t<decltype(v)>::Type;

        if constexpr (std::is_same_v<V, bool>) {
          return sqldb::Value{
              static_cast<bool>(sqlite3_column_int(&statement, index))};
        } else if constexpr (std::is_same_v<V, int>) {
          return sqldb::Value{sqlite3_column_int(&statement, index)};
        } else if constexpr (std::is_same_v<V, int64_t>) {
          return sqldb::Value{int64_t{sqlite3_column_int64(&statement, index)}};
        } else if constexpr (std::is_same_v<V, double>) {
          return sqldb::Value{sqlite3_column_double(&statement, index)};
        } else if constexpr (std::is_same_v<V, std::string>) {
          // NOLINTNEXTLINE(*-reinterpret-cast)
          return sqldb::Value{std::string{reinterpret_cast<const char *>(
              sqlite3_column_text(&statement, index))}};
        } else {
          Expects(false);
        }
      },
      *type);
}
}  // namespace

void NativeStatementFacade::Reset(sqlite3_stmt &native_statement) {
  const auto result_code = sqlite3_reset(&native_statement);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{
        fmt::format("Couldn't reset prepared statement: {}", result_code)};
  }
}

void NativeStatementFacade::BindParams(
    sqlite3_stmt &native_statement, const std::vector<sqldb::Value> &params) {
  for (auto i = 0; i < gsl::narrow_cast<int>(params.size()); ++i) {
    BindParam(native_statement, i + 1, params[i]);
  }
}

auto NativeStatementFacade::Step(sqlite3_stmt &native_statement) -> ResultCode {
  return {sqlite3_step(&native_statement)};
}

auto NativeStatementFacade::GetStepValues(
    sqlite3_stmt &native_statement,
    const std::vector<sqldb::DataTypeVariant> &value_types)
    -> std::vector<sqldb::Value> {
  const auto num_values = value_types.size();

  auto values = std::vector<sqldb::Value>{};
  values.reserve(num_values);

  for (auto i = 0; i < gsl::narrow_cast<int>(num_values); ++i) {
    values.emplace_back(GetValue(native_statement, i, value_types[i]));
  }

  return values;
}

void NativeStatementFacade::Finalize(sqlite3_stmt &native_statement) {
  const auto result_code = sqlite3_finalize(&native_statement);

  if (result_code != SQLITE_OK) {
    throw cpp::MessageException{
        fmt::format("Couldn't finalize prepared statement: {}", result_code)};
  }
}
}  // namespace stonks::sqlite