find_package(fmt REQUIRED)

add_library(libcore_symbol_prices_stream
  core_sps_endpoints.cc
  core_sps_json_user_conversions.cc
  core_symbol_prices_stream.cc
)

set_target_properties(libcore_symbol_prices_stream PROPERTIES
  OUTPUT_NAME core_symbol_prices_stream
)

target_include_directories(libcore_symbol_prices_stream
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/core/symbol_prices_stream
)

target_link_libraries(libcore_symbol_prices_stream
  PUBLIC libcore_symbols_db
  PUBLIC libnetwork

  PRIVATE fmt::fmt
)