find_package(absl REQUIRED)

add_library(libservice
  service_json_conversions.cc
)

set_target_properties(libservice PROPERTIES
  OUTPUT_NAME service
)

target_include_directories(libservice
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/service
)

target_link_libraries(libservice
  PRIVATE absl::absl
  PRIVATE libcore
  PRIVATE libnetwork
)

add_subdirectory(symbol_prices_stream)
add_subdirectory(symbols_db)
add_subdirectory(symbols_info_updater)