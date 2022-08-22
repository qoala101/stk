find_package(SQLite3 REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(range-v3 REQUIRED)
find_package(spdlog REQUIRED)
find_package(magic_enum REQUIRED)

add_library(libsqlite
  sqlite_db_facade.cc
  sqlite_factory.cc
  sqlite_db.cc
  sqlite_prepared_statement_facade.cc
  sqlite_prepared_statement_handle.cc
  sqlite_query_builder.cc
  sqlite_read_from_file.cc
  sqlite_select_statement.cc
  sqlite_types.cc
  sqlite_update_statement.cc
)

set_target_properties(libsqlite PROPERTIES
  OUTPUT_NAME sqlite
)

target_include_directories(libsqlite
  PUBLIC ${PROJECT_SOURCE_DIR}/include/sqlite
)

target_link_libraries(libsqlite
  PUBLIC libsqldb
  PRIVATE SQLite::SQLite3
  PRIVATE Microsoft.GSL::GSL
  PRIVATE range-v3::range-v3
  PRIVATE spdlog::spdlog
  PRIVATE magic_enum::magic_enum
)