find_package(fmt REQUIRED)

add_library(libservice_symbol_prices_streams_controller
  service_spsc_options.cc
  service_symbol_prices_streams_controller.cc
)

set_target_properties(libservice_symbol_prices_streams_controller PROPERTIES
  OUTPUT_NAME service_symbol_prices_streams_controller
)

target_include_directories(libservice_symbol_prices_streams_controller
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/service/symbol_prices_streams_controller
)

target_link_libraries(libservice_symbol_prices_streams_controller
  PUBLIC libcli
  PUBLIC libcore_symbol_prices_streams_controller
  PUBLIC libnetworkx
  PUBLIC fmt::fmt

  PRIVATE libservice
)

add_executable(service_symbol_prices_streams_controller
  main.cc
)

target_link_libraries(service_symbol_prices_streams_controller
  PRIVATE libnetwork
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libservice_symbol_prices_streams_controller
  PRIVATE libspdlog
)