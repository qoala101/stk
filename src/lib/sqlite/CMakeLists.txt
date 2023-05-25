add_library(libsqlite
  native/sqlite_native_db_facade.cc
  native/sqlite_native_db_file_handle.cc
  native/sqlite_native_db_handle_variant.cc
  native/sqlite_native_db_handles_factory.cc
  native/sqlite_native_handles.cc
  native/sqlite_native_statement_facade.cc
  sqlite_db.cc
  sqlite_prepared_statement_impl.cc
  sqlite_select_statement.cc
  sqlite_update_statement.cc
)
add_library(vh::libsqlite ALIAS libsqlite)

set_target_properties(libsqlite PROPERTIES
  OUTPUT_NAME sqlite
)

target_include_directories(libsqlite
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include/lib/sqlite
  ${PROJECT_SOURCE_DIR}/include/lib/sqlite/native
)

target_link_libraries(libsqlite
  PUBLIC
  vh::liblog
  vh::libsqldb

  PRIVATE
  fmt::fmt
  range-v3::range-v3
  SQLite::SQLite3
)