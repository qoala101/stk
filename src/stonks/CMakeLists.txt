find_package(Microsoft.GSL REQUIRED)
find_package(range-v3 REQUIRED)

add_library(libstonks
  db/stonks_cache.cc
  db/stonks_table_definitions.cc
  db/stonks_db.cc
  db/stonks_prepared_statements.cc
)

set_target_properties(libstonks PROPERTIES
  OUTPUT_NAME stonks
)

target_include_directories(libstonks
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stonks
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stonks/db
)

target_link_libraries(libstonks
  PRIVATE librestsdk
  PRIVATE libsqlite
  PRIVATE Microsoft.GSL::GSL
  PRIVATE range-v3::range-v3
)