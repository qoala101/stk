add_library(libsqldb
  parametrized/sqldb_prm_common.cc
  parametrized/sqldb_prm_db.cc
  parametrized/sqldb_prm_select_statement.cc
  parametrized/sqldb_prm_types.cc
  parametrized/sqldb_prm_update_statement.cc
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
add_library(vh::libsqldb ALIAS libsqldb)

set_target_properties(libsqldb PROPERTIES
  OUTPUT_NAME sqldb
)

target_include_directories(libsqldb
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/sqldb
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/sqldb/parametrized
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/sqldb/query_builder
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/sqldb/thread_safe
)

target_link_libraries(libsqldb
  PUBLIC nameof::nameof
  PUBLIC vh::libcpp
  PRIVATE fmt::fmt
  PRIVATE magic_enum::magic_enum
  PRIVATE range-v3::range-v3
)