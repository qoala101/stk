add_library(libcore_buy_low_sell_high
  core_buy_low_sell_high.cc
)

set_target_properties(libcore_buy_low_sell_high PROPERTIES
  OUTPUT_NAME core_buy_low_sell_high
)

target_include_directories(libcore_buy_low_sell_high
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/core/buy_low_sell_high
)

target_link_libraries(libcore_buy_low_sell_high
  PUBLIC libcore_symbols_db
)