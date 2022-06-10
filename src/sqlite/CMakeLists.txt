find_package(SQLite3 REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(spdlog REQUIRED)
find_package(magic_enum REQUIRED)
find_package(range-v3 REQUIRED)
find_package(absl REQUIRED)

add_library(libsqlite
  sqlite_db_factory.cc
  sqlite_db.cc
  sqlite_prepared_statement.cc
  sqlite_query_builder.cc
  sqlite_utils.cc
)
set_target_properties(libsqlite PROPERTIES OUTPUT_NAME sqlite)
target_include_directories(libsqlite PUBLIC ${PROJECT_SOURCE_DIR}/include/sqlite)
target_link_libraries(libsqlite PUBLIC libsqldb)
target_link_libraries(libsqlite PRIVATE SQLite::SQLite3)
target_link_libraries(libsqlite PRIVATE Microsoft.GSL::GSL)
target_link_libraries(libsqlite PRIVATE spdlog::spdlog)
target_link_libraries(libsqlite PRIVATE magic_enum::magic_enum)
target_link_libraries(libsqlite PRIVATE range-v3::range-v3)
target_link_libraries(libsqlite PRIVATE absl::absl)