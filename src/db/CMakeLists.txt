find_package(SQLite3 REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(spdlog REQUIRED)
find_package(magic_enum REQUIRED)
find_package(range-v3 REQUIRED)
find_package(absl REQUIRED)

add_library(libdb
  sqlite/sqlite_db.cc
  sqlite/sqlite_prepared_statement.cc
  sqlite/sqlite_query_builder.cc
  db_row_definition.cc
  db_row.cc
  db_value.cc
)
set_target_properties(libdb PROPERTIES OUTPUT_NAME db)
target_include_directories(libdb PUBLIC ${PROJECT_SOURCE_DIR}/include/db)
target_include_directories(libdb PUBLIC ${PROJECT_SOURCE_DIR}/include/db/sqlite)
target_link_libraries(libdb PRIVATE SQLite::SQLite3)
target_link_libraries(libdb PUBLIC Microsoft.GSL::GSL)
target_link_libraries(libdb PRIVATE spdlog::spdlog)
target_link_libraries(libdb PRIVATE magic_enum::magic_enum)
target_link_libraries(libdb PRIVATE range-v3::range-v3)
target_link_libraries(libdb PRIVATE absl::absl)