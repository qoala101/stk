add_library(libcore_buy_low_sell_high
  core_buy_low_sell_high.cc
)

set_target_properties(libcore_buy_low_sell_high PROPERTIES
  OUTPUT_NAME core_buy_low_sell_high
)

target_include_directories(libcore_buy_low_sell_high
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/core/buy_low_sell_high
)

target_link_libraries(libcore_buy_low_sell_high
  PRIVATE libcore
  PRIVATE libcore_symbols_db
  PRIVATE libcpp

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE fmt::fmt
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
)