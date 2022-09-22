find_package(range-v3 REQUIRED)

add_library(libstonks
  db/stonks_db_cache.cc
  db/stonks_db_prepared_statements.cc
  db/stonks_db_tables.cc
  db/stonks_db.cc
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
  PRIVATE range-v3::range-v3
)