add_library(libcore
  core_common.cc
  core_json_conversions.cc
)

set_target_properties(libcore PROPERTIES
  OUTPUT_NAME core
)

target_include_directories(libcore
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/core
)

target_link_libraries(libcore
  PRIVATE libcpp
  PRIVATE libnetwork

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE magic_enum::magic_enum
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
)

add_subdirectory(buy_low_sell_high)
add_subdirectory(old_symbol_prices_deleter)
add_subdirectory(symbol_price_streams)
add_subdirectory(symbol_prices_filter)
add_subdirectory(symbols_db)
add_subdirectory(symbols_info_updater)