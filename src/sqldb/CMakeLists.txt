find_package(range-v3 REQUIRED)
find_package(Microsoft.GSL REQUIRED)

add_library(libsqldb
  sqldb_query_builder.cc
  sqldb_row_definition.cc
  sqldb_row.cc
  sqldb_rows.cc
  sqldb_select_statement.cc
  sqldb_types.cc
  sqldb_update_statement.cc
  sqldb_value.cc
)
set_target_properties(libsqldb PROPERTIES OUTPUT_NAME sqldb)
target_include_directories(libsqldb PUBLIC ${PROJECT_SOURCE_DIR}/include/sqldb)
target_link_libraries(libsqldb PRIVATE range-v3::range-v3)
target_link_libraries(libsqldb PRIVATE Microsoft.GSL::GSL)