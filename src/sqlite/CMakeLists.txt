add_library(libsqlite
  sqlite_db_facade.cc
  sqlite_db_file_handle.cc
  sqlite_db_handle_variant.cc
  sqlite_db_handles_factory.cc
  sqlite_db.cc
  sqlite_prepared_statement_facade.cc
  sqlite_prepared_statement_handle.cc
  sqlite_ps_common_impl.cc
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
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE liblog
  PRIVATE libsqldb

  PRIVATE callable
  PRIVATE di::di
  PRIVATE fmt::fmt
  PRIVATE function2::function2
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
  PRIVATE range-v3::range-v3
  PRIVATE SQLite::SQLite3
)