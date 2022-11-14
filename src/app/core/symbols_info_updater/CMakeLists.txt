find_package(range-v3 REQUIRED)

add_library(libcore_symbols_info_updater
  core_siu_binance_client.cc
  core_siu_endpoints.cc
  core_siu_json_conversions.cc
  core_symbols_info_updater.cc
)

set_target_properties(libcore_symbols_info_updater PROPERTIES
  OUTPUT_NAME core_symbols_info_updater
)

target_include_directories(libcore_symbols_info_updater
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/core/symbols_info_updater
)

target_link_libraries(libcore_symbols_info_updater
  PUBLIC libcore_symbols_db
  PUBLIC libnetwork

  PRIVATE range-v3::range-v3
)