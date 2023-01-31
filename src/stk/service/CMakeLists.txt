add_library(libservice
  service_client_options.cc
  service_server_options.cc
)

set_target_properties(libservice PROPERTIES
  OUTPUT_NAME service
)

target_include_directories(libservice
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/service
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/service/injector
)

target_link_libraries(libservice
  PUBLIC libaws
  PUBLIC libcli
  PUBLIC libdi
  PUBLIC libnetworkx
  PUBLIC librestsdk
  PUBLIC libspdlog
  PUBLIC libsqlite

  PRIVATE fmt::fmt
)

add_subdirectory(app_uri_exposer)
add_subdirectory(buy_low_sell_high)
add_subdirectory(old_symbol_prices_deleter)
add_subdirectory(symbol_price_streams)
add_subdirectory(symbol_prices_filter)
add_subdirectory(symbols_db)
add_subdirectory(symbols_info_updater)