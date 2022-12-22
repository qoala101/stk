find_package(fmt REQUIRED)

add_library(libcore_symbol_prices_stream
  core_sps_book_tick_handler.cc
  core_sps_book_tick_web_socket_factory.cc
  core_symbol_prices_stream.cc
)

set_target_properties(libcore_symbol_prices_stream PROPERTIES
  OUTPUT_NAME core_symbol_prices_stream
)

target_include_directories(libcore_symbol_prices_stream
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/core/symbol_prices_stream
)

target_link_libraries(libcore_symbol_prices_stream
  PUBLIC libbinance
  PUBLIC libcore_symbol_prices_streams_controller
  PUBLIC libcore_symbols_db
  PUBLIC libnetworkx

  PRIVATE fmt::fmt
)