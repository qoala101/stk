find_package(magic_enum REQUIRED)
find_package(range-v3 REQUIRED)

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

set_target_properties(libsqldb PROPERTIES
  OUTPUT_NAME sqldb
)

target_include_directories(libsqldb
  PUBLIC ${PROJECT_SOURCE_DIR}/include/sqldb
  PUBLIC ${PROJECT_SOURCE_DIR}/include/sqldb/query_builders
)

target_link_libraries(libsqldb
  PUBLIC libcpp
  PUBLIC magic_enum::magic_enum
  PRIVATE range-v3::range-v3
)