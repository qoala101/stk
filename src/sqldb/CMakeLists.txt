find_package(absl REQUIRED)
find_package(fmt REQUIRED)
find_package(function2 REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(nameof REQUIRED)
find_package(range-v3 REQUIRED)

add_library(libsqldb
  parametrized/sqldb_p_common.cc
  parametrized/sqldb_p_db.cc
  parametrized/sqldb_p_select_statement.cc
  parametrized/sqldb_p_types.cc
  parametrized/sqldb_p_update_statement.cc
  query_builder/sqldb_qb_common.cc
  query_builder/sqldb_qb_condition.cc
  query_builder/sqldb_qb_create.cc
  query_builder/sqldb_qb_delete.cc
  query_builder/sqldb_qb_drop.cc
  query_builder/sqldb_qb_insert.cc
  query_builder/sqldb_qb_query_value.cc
  query_builder/sqldb_qb_query_wrapper.cc
  query_builder/sqldb_qb_select.cc
  query_builder/sqldb_qb_types.cc
  query_builder/sqldb_qb_update.cc
  query_builder/sqldb_qb_wrapped_conditions.cc
  sqldb_i_db.cc
  sqldb_i_select_statement.cc
  sqldb_i_update_statement.cc
  sqldb_query_builder.cc
  sqldb_rows.cc
  sqldb_table.cc
  sqldb_types.cc
  sqldb_value.cc
)

set_target_properties(libsqldb PROPERTIES
  OUTPUT_NAME sqldb
)

target_include_directories(libsqldb
  PUBLIC ${PROJECT_SOURCE_DIR}/include/sqldb
  PUBLIC ${PROJECT_SOURCE_DIR}/include/sqldb/parametrized
  PUBLIC ${PROJECT_SOURCE_DIR}/include/sqldb/query_builder
)

target_link_libraries(libsqldb
  PRIVATE libcpp
  PRIVATE libdi

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE fmt::fmt
  PRIVATE function2::function2
  PRIVATE Microsoft.GSL::GSL
  PRIVATE nameof::nameof
  PRIVATE NotNull
  PRIVATE range-v3::range-v3
)