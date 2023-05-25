add_library(libbinance
  binance_api.cc
  binance_json_conversions.cc
)
add_library(vh::libbinance ALIAS libbinance)

set_target_properties(libbinance PROPERTIES
  OUTPUT_NAME binance
)

target_include_directories(libbinance
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include/lib/binance
)

target_link_libraries(libbinance
  PUBLIC
  vh::libnetworkx
)