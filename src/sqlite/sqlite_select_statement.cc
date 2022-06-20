#include "sqlite_select_statement.h"

#include <sqlite3.h>

#include <gsl/assert>
#include <memory>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <stdexcept>
#include <string>
#include <utility>

#include "sqldb_types.h"
#include "sqlite_prepared_statement_facade.h"

namespace stonks::sqlite {
SqliteSelectStatement::SqliteSelectStatement(
    SqlitePreparedStatementHandle &&prepared_statement_handle,
    const sqldb::RowDefinition &result_definition)
    : prepared_statement_handle_{std::move(prepared_statement_handle)},
      result_columns_{result_definition.GetCellDefinitions() |
                      ranges::views::transform([](const auto &cell_definition) {
                        return cell_definition.column;
                      }) |
                      ranges::to_vector},
      result_types_{result_definition.GetCellDefinitions() |
                    ranges::views::transform([](const auto &cell_definition) {
                      return cell_definition.data_type;
                    }) |
                    ranges::to_vector} {
  Expects(prepared_statement_handle_.IsValid());
  Ensures(result_columns_.size() == result_types_.size());
}

auto SqliteSelectStatement::Execute(const std::vector<sqldb::Value> &params)
    -> sqldb::Rows {
  auto sqlite_statement = prepared_statement_handle_.GetSqliteStatement();
  Expects(sqlite_statement != nullptr);

  auto result_rows = sqldb::Rows{result_columns_};

  auto prepared_statement_facade =
      SqlitePreparedStatementFacade{*sqlite_statement};
  prepared_statement_facade.Reset();
  prepared_statement_facade.BindParams(params);

  while (true) {
    const auto result_code = prepared_statement_facade.Step();

    switch (result_code) {
      case SQLITE_ROW:
        result_rows.Push(
            prepared_statement_facade.GetStepValues(result_types_));
        continue;

      case SQLITE_DONE:
        return result_rows;

      default:
        throw std::runtime_error{"Unexpected select statement result: " +
                                 std::to_string(result_code)};
    }
  }
}
}  // namespace stonks::sqlite