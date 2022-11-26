find_package(cppcoro REQUIRED)
find_package(range-v3 REQUIRED)

add_library(libcore_symbols_db
  core_sdb_value_conversions.cc
  core_symbols_db.cc
)

set_target_properties(libcore_symbols_db PROPERTIES
  OUTPUT_NAME core_symbols_db
)

target_include_directories(libcore_symbols_db
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/core/symbols_db
)

target_link_libraries(libcore_symbols_db
  PUBLIC cppcoro::cppcoro
  PUBLIC libcore
  PUBLIC libsqldb

  PRIVATE range-v3::range-v3
)