add_library(libcore_symbols_db
  core_sdb_proxy.cc
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
  PRIVATE libcore
  PRIVATE libcpp
  PRIVATE libsqldb

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE function2::function2
  PRIVATE Microsoft.GSL::GSL
  PRIVATE nameof::nameof
  PRIVATE NotNull
  PRIVATE range-v3::range-v3
)