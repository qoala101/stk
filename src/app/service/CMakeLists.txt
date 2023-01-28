add_library(libservice
  service_client_options.cc
  service_server_options.cc
)

set_target_properties(libservice PROPERTIES
  OUTPUT_NAME service
)

target_include_directories(libservice
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/service
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/service/injector
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/service/injector/thread_safe
)

target_link_libraries(libservice
  PRIVATE libcli
  PRIVATE libcpp
  PRIVATE libnetwork
  PRIVATE libnetworkx

  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE fmt::fmt
  PRIVATE function2::function2
  PRIVATE magic_enum::magic_enum
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
)

add_subdirectory(app_uri_exposer)
add_subdirectory(buy_low_sell_high)
add_subdirectory(old_symbol_prices_deleter)
add_subdirectory(symbol_price_streams)
add_subdirectory(symbol_prices_filter)
add_subdirectory(symbols_db)
add_subdirectory(symbols_info_updater)