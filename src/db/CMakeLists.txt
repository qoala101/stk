find_package(range-v3 REQUIRED)
find_package(absl REQUIRED)

add_library(libdb
  db_prepared_statement.cc
  db_query_builder.cc
  db_row_definition.cc
  db_row.cc
  db_rows.cc
  db_value.cc
)
set_target_properties(libdb PROPERTIES OUTPUT_NAME db)
target_include_directories(libdb PUBLIC ${PROJECT_SOURCE_DIR}/include/db)
target_link_libraries(libdb PRIVATE range-v3::range-v3)
target_link_libraries(libdb PRIVATE absl::absl)