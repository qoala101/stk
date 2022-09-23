find_package(fmt REQUIRED)
find_package(range-v3 REQUIRED)
find_package(SQLite3 REQUIRED)

add_library(libsqlite
  sqlite_db_facade.cc
  sqlite_db_file_handle.cc
  sqlite_db_handle_variant.cc
  sqlite_db_handles_factory.cc
  sqlite_db.cc
  sqlite_prepared_statement_facade.cc
  sqlite_prepared_statement_handle.cc
  sqlite_query_builder.cc
  sqlite_raw_handles.cc
  sqlite_select_statement.cc
  sqlite_update_statement.cc
)

set_target_properties(libsqlite PROPERTIES
  OUTPUT_NAME sqlite
)

target_include_directories(libsqlite
  PUBLIC ${PROJECT_SOURCE_DIR}/include/sqlite
)

target_link_libraries(libsqlite
  PUBLIC liblog
  PUBLIC libsqldb

  PRIVATE fmt::fmt
  PRIVATE range-v3::range-v3
  PRIVATE SQLite::SQLite3
)