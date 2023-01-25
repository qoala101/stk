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

set_target_properties(libsqlite PROPERTIES
  OUTPUT_NAME sqlite
)

target_include_directories(libsqlite
  PUBLIC ${PROJECT_SOURCE_DIR}/include/sqlite
  PUBLIC ${PROJECT_SOURCE_DIR}/include/sqlite/native
)

target_link_libraries(libsqlite
  PRIVATE libcpp
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