find_package(absl REQUIRED)
find_package(fmt REQUIRED)
find_package(range-v3 REQUIRED)

add_library(libsqldb
  query_builder/sqldb_qb_common.cc
  query_builder/sqldb_qb_condition.cc
  query_builder/sqldb_qb_create.cc
  query_builder/sqldb_qb_delete.cc
  query_builder/sqldb_qb_drop.cc
  query_builder/sqldb_qb_insert.cc
  query_builder/sqldb_qb_select.cc
  query_builder/sqldb_qb_update.cc
  sqldb_i_select_statement.cc
  sqldb_i_update_statement.cc
  sqldb_query_builder.cc
  sqldb_rows.cc
  sqldb_value.cc
)

set_target_properties(libsqldb PROPERTIES
  OUTPUT_NAME sqldb
)

target_include_directories(libsqldb
  PUBLIC ${PROJECT_SOURCE_DIR}/include/sqldb
  PUBLIC ${PROJECT_SOURCE_DIR}/include/sqldb/query_builder
)

target_link_libraries(libsqldb
  PUBLIC libcpp
  PUBLIC libdi

  PRIVATE absl::absl
  PRIVATE fmt::fmt
  PRIVATE range-v3::range-v3
)