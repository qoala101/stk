find_package(range-v3 REQUIRED)
find_package(Microsoft.GSL REQUIRED)

add_library(libsqldb
  query_builders/sqldb_delete_query_builder.cc
  query_builders/sqldb_insert_query_builder.cc
  query_builders/sqldb_query_builders_common.cc
  query_builders/sqldb_select_query_builder.cc
  query_builders/sqldb_update_query_builder.cc
  sqldb_i_select_statement.cc
  sqldb_i_update_statement.cc
  sqldb_query_builder_facade.cc
  sqldb_row_definition.cc
  sqldb_row.cc
  sqldb_rows.cc
  sqldb_types.cc
  sqldb_value.cc
)
set_target_properties(libsqldb PROPERTIES OUTPUT_NAME sqldb)
target_include_directories(libsqldb PUBLIC ${PROJECT_SOURCE_DIR}/include/sqldb)
target_include_directories(libsqldb PUBLIC ${PROJECT_SOURCE_DIR}/include/sqldb/query_builders)
target_include_directories(libsqldb PUBLIC ${PROJECT_SOURCE_DIR}/include/ccutils)
target_link_libraries(libsqldb PRIVATE range-v3::range-v3)
target_link_libraries(libsqldb PRIVATE Microsoft.GSL::GSL)
target_link_libraries(libsqldb PUBLIC NotNull)