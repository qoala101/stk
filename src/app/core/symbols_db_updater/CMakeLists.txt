find_package(absl REQUIRED)
find_package(cppcoro REQUIRED)
find_package(di REQUIRED)
find_package(function2 REQUIRED)
find_package(polymorphic_value REQUIRED)
find_package(range-v3 REQUIRED)

add_library(libcore_symbols_db_updater
  core_sdbu_old_prices_deleter.cc
  core_sdbu_symbols_info_updater.cc
  core_symbols_db_updater.cc
)

set_target_properties(libcore_symbols_db_updater PROPERTIES
  OUTPUT_NAME core_symbols_db_updater
)

target_include_directories(libcore_symbols_db_updater
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/core/symbols_db_updater
)

target_link_libraries(libcore_symbols_db_updater
  PRIVATE libbinance
  PRIVATE libcore
  PRIVATE libcore_symbols_db
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE libnetwork

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE di::di
  PRIVATE function2::function2
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
  PRIVATE range-v3::range-v3
)