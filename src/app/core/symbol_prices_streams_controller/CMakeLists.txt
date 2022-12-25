add_library(libcore_symbol_prices_streams_controller
  core_symbol_prices_streams_controller.cc
)

set_target_properties(libcore_symbol_prices_streams_controller PROPERTIES
  OUTPUT_NAME core_symbol_prices_streams_controller
)

target_include_directories(libcore_symbol_prices_streams_controller
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/core/symbol_prices_streams_controller
)

target_link_libraries(libcore_symbol_prices_streams_controller
  PRIVATE libcore
  PRIVATE libcpp
  PRIVATE liblog

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE fmt::fmt
  PRIVATE NotNull
)