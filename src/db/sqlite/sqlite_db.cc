#include "sqlite_db.h"

#include <spdlog/spdlog.h>
#include <sqlite3.h>

#include <gsl/util>
#include <range/v3/all.hpp>

#include "db_enum_conversions.h"

namespace stonks::db::sqlite {

std::optional<std::string> GetDbFileName(sqlite3 &sqlite_db_handle) {
  const auto file_name = sqlite3_db_filename(&sqlite_db_handle, nullptr);

  if (file_name == nullptr) {
    return std::nullopt;
  }

  return file_name;
}

std::optional<SqliteDb> SqliteDb::OpenOrCreateDbFromFile(
    std::string_view file_name) {
  auto sqlite_db_handle = (sqlite3 *){nullptr};
  const auto result_code = sqlite3_open(file_name.data(), &sqlite_db_handle);
  assert(sqlite_db_handle != nullptr);

  auto sqlite_db = SqliteDb{sqlite_db_handle};

  if (result_code != SQLITE_OK) {
    spdlog::error("Cannot open DB from {}: {}", file_name,
                  sqlite3_errmsg(sqlite_db.sqlite_db_handle_));
    return std::nullopt;
  }

  const auto full_file_name = GetDbFileName(*sqlite_db.sqlite_db_handle_);
  assert(full_file_name.has_value());

  spdlog::info("Opened DB from {}", *full_file_name);
  return std::optional<SqliteDb>{std::move(sqlite_db)};
}

void swap(SqliteDb &left, SqliteDb &right) {
  using std::swap;
  swap(left.sqlite_db_handle_, right.sqlite_db_handle_);
}

SqliteDb::SqliteDb(SqliteDb &&other) { swap(*this, other); }

SqliteDb &SqliteDb::operator=(SqliteDb &&other) {
  auto temp = SqliteDb{std::move(other)};
  swap(*this, temp);
  return *this;
}

SqliteDb::~SqliteDb() {
  if (sqlite_db_handle_ == nullptr) {
    return;
  }

  const auto db_file_name = GetDbFileName(*sqlite_db_handle_);
  const auto result_code = sqlite3_close(sqlite_db_handle_);

  if (result_code != SQLITE_OK) {
    spdlog::error("Cannot close DB from {}",
                  db_file_name.value_or("UNKNOWN_FILE"));
    return;
  }

  spdlog::info("Closed DB from {}", db_file_name.value_or("UNKNOWN_FILE"));
}

namespace {
std::optional<std::string> ExecuteQuery(sqlite3 &sqlite_db_handle,
                                        std::string_view query) {
  auto error_message = (char *){nullptr};
  const auto finally_free_error_message =
      gsl::finally([&error_message]() { sqlite3_free(error_message); });

  spdlog::info("Executing query: {}", query);
  const auto result_code = sqlite3_exec(&sqlite_db_handle, query.data(),
                                        nullptr, nullptr, &error_message);

  if (result_code != SQLITE_OK) {
    return error_message;
  }

  return std::nullopt;
}
}  // namespace

bool SqliteDb::CreateTable(const TableDefinition &table_definition) {
  const auto column_is_primary_key = [](const Column &column) {
    return column.primary_key == true;
  };
  const auto has_primary_keys =
      ranges::any_of(table_definition.columns, column_is_primary_key);

  const auto column_is_foreign_key = [](const Column &column) {
    return column.foreign_key.has_value();
  };
  const auto has_foreign_keys =
      ranges::any_of(table_definition.columns, column_is_foreign_key);

  const auto has_keys = has_primary_keys || has_foreign_keys;

  auto query =
      std::string{"CREATE TABLE \"" + table_definition.table.name + "\"(\n"};

  for (const auto &column : table_definition.columns) {
    query += "\t\"" + column.name + "\" " +
             std::string{magic_enum::enum_name(column.data_type)} + " NOT NULL";

    if (column.unique) {
      query += " UNIQUE";
    }

    const auto last_column = &column == &table_definition.columns.back();

    if (!last_column || has_keys) {
      query += ",\n";
    }
  }

  if (has_primary_keys) {
    query += "PRIMARY KEY(";
    auto primary_key_columns =
        table_definition.columns | ranges::views::filter(column_is_primary_key);

    for (const auto &column : primary_key_columns) {
      query += "\"" + column.name + "\"";

      if (column.auto_increment) {
        query += " AUTOINCREMENT";
      }

      const auto last_column = &column == &primary_key_columns.back();

      if (!last_column) {
        query += ", ";
      }
    }

    query += ")";

    if (has_foreign_keys) {
      query += ",\n";
    }
  }

  if (has_foreign_keys) {
    auto foreign_key_columns =
        table_definition.columns | ranges::views::filter(column_is_primary_key);

    for (const auto &column : foreign_key_columns) {
      const auto &foreign_key = *column.foreign_key;
      query += "FOREIGN KEY(\"" + column.name + "\") REFERENCES \"" +
               foreign_key.table_name + "\"(\"" + foreign_key.column_name +
               "\")";

      const auto last_column = &column == &foreign_key_columns.back();

      if (!last_column) {
        query += ",\n";
      }
    }
  }

  query += "\n);";

  const auto error_message = ExecuteQuery(*sqlite_db_handle_, query);

  if (error_message.has_value()) {
    spdlog::error("Cannot create table: {}", *error_message);
    return false;
  }

  return true;
}

bool SqliteDb::DropTable(const Table &table) {
  const auto query = std::string{"DROP TABLE \"" + table.name + "\";"};
  const auto error_message = ExecuteQuery(*sqlite_db_handle_, query);

  if (error_message.has_value()) {
    spdlog::error("Cannot drop table: {}", *error_message);
    return false;
  }

  return true;
}

bool SqliteDb::Insert(const Table &table, const Row &row) { return false; }

std::optional<std::vector<Row>> SqliteDb::Select(const Table &table) {
  return {};
}

SqliteDb::SqliteDb(sqlite3 *sqlite_db_handle)
    : sqlite_db_handle_{sqlite_db_handle} {}
}  // namespace stonks::db::sqlite