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
  PUBLIC libcore
  PUBLIC libnetwork
)

add_subdirectory(app_uri_exposer)
add_subdirectory(buy_low_sell_high)
add_subdirectory(symbol_prices_stream)
add_subdirectory(symbols_db)
add_subdirectory(symbols_db_updater)