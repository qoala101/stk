add_library(libbinance
  binance_api.cc
  binance_json_conversions.cc
)

set_target_properties(libbinance PROPERTIES
  OUTPUT_NAME binance
)

target_include_directories(libbinance
  PUBLIC ${PROJECT_SOURCE_DIR}/include/binance
)

target_link_libraries(libbinance
  PUBLIC libnetwork

  PRIVATE libnetworkx
)