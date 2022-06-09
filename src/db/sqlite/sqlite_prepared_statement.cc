#include "sqlite_prepared_statement.h"

#include <absl/base/macros.h>
#include <sqlite3.h>

#include <cstdint>
#include <gsl/assert>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/transform.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "db_enums.h"
#include "db_row.h"
#include "db_types.h"

namespace stonks::db::sqlite {
namespace {
void BindParam(sqlite3_stmt &statement, int index, const Value &value) {
  if (value.IsNull()) {
    return;
  }

  switch (value.GetType()) {
    case DataType::kInt:
      sqlite3_bind_int(&statement, index, value.GetInt());
      return;
    case DataType::kInt64:
      sqlite3_bind_int64(&statement, index, value.GetInt64());
      return;
    case DataType::kDouble:
      sqlite3_bind_double(&statement, index, value.GetDouble());
      return;
    case DataType::kString: {
      const auto &string = value.GetString();
      sqlite3_bind_text(&statement, index, string.c_str(),
                        static_cast<int>(string.length()), nullptr);
      return;
    }
  }
}

auto GetValue(sqlite3_stmt &statement, int index, DataType type) -> Value {
  if (sqlite3_column_type(&statement, index) == SQLITE_NULL) {
    return {};
  }

  switch (type) {
    case DataType::kInt:
      return Value{sqlite3_column_int(&statement, index)};
    case DataType::kInt64:
      return Value{int64_t{sqlite3_column_int64(&statement, index)}};
    case DataType::kDouble:
      return Value{sqlite3_column_double(&statement, index)};
    case DataType::kString:
      // NOLINTNEXTLINE(*-reinterpret-cast)
      return Value{reinterpret_cast<const char *>(
          sqlite3_column_text(&statement, index))};
  }
}

auto GetValues(sqlite3_stmt &statement,
               const std::vector<CellDefinition> &cell_definitions)
    -> std::vector<Value> {
  const auto num_columns = cell_definitions.size();

  auto values = std::vector<Value>{};
  values.reserve(num_columns);

  for (auto i = 0; i < num_columns; ++i) {
    values.emplace_back(GetValue(statement, i, cell_definitions[i].data_type));
  }

  return values;
}
}  // namespace

class SqlitePreparedStatement::Impl {
 public:
  explicit Impl(std::weak_ptr<sqlite3_stmt> statement,
                std::function<void(sqlite3_stmt &)> finalize_statement_callback)
      : statement_{std::move(statement)},
        finalize_statement_callback_{std::move(finalize_statement_callback)} {
    Expects(finalize_statement_callback_);
  }

  Impl(const Impl &) = delete;
  Impl(Impl &&) noexcept = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) noexcept -> Impl & = default;

  ~Impl() {
    if (const auto statement = statement_.lock()) {
      finalize_statement_callback_(*statement);
    }
  }

  auto Execute(const std::vector<Value> &params,
               const RowDefinition &result_definition) -> Rows {
    const auto statement = statement_.lock();
    ABSL_ASSERT((statement != nullptr) &&
                "Trying to execute finalized statement");

    auto *const statement_ptr = statement.get();
    const auto reset_result = sqlite3_reset(statement_ptr);

    for (auto i = 0; i < params.size(); ++i) {
      BindParam(*statement_ptr, i + 1, params[i]);
    }

    const auto &cell_definitions = result_definition.GetCellDefinitions();
    auto columns =
        ranges::views::transform(cell_definitions,
                                 [](const auto &cell) { return cell.column; }) |
        ranges::to_vector;
    auto rows = Rows{std::move(columns)};
    auto num_received_columns = -1;

    while (true) {
      const auto result_code = sqlite3_step(statement_ptr);

      switch (result_code) {
        case SQLITE_ROW: {
          if (const auto num_received_columns_not_set =
                  num_received_columns < 0) {
            num_received_columns = sqlite3_column_count(statement_ptr);

            if (num_received_columns != cell_definitions.size()) {
              throw std::runtime_error{
                  "Statement result has wrong amount of columns"};
            }
          }

          rows.Push(GetValues(*statement_ptr, cell_definitions));
        } break;

        case SQLITE_DONE:
          return rows;

        default:
          throw std::runtime_error{
              "Unexpected prepared statement step result: " +
              std::to_string(result_code)};
      }
    }
  }

 private:
  std::weak_ptr<sqlite3_stmt> statement_{};
  std::function<void(sqlite3_stmt &)> finalize_statement_callback_{};
};

SqlitePreparedStatement::SqlitePreparedStatement(
    std::weak_ptr<sqlite3_stmt> statement,
    std::function<void(sqlite3_stmt &)> finalize_statement_callback)
    : impl_{std::make_unique<Impl>(std::move(statement),
                                   std::move(finalize_statement_callback))} {}

SqlitePreparedStatement::~SqlitePreparedStatement() = default;

auto SqlitePreparedStatement::Execute(const std::vector<Value> &params,
                                      const RowDefinition &result_definition)
    -> Rows {
  return impl_->Execute(params, result_definition);
}
}  // namespace stonks::db::sqlite