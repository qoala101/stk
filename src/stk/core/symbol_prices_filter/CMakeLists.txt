add_library(libcore_symbol_prices_filter
  core_symbol_prices_filter.cc
)

set_target_properties(libcore_symbol_prices_filter PROPERTIES
  OUTPUT_NAME core_symbol_prices_filter
)

target_include_directories(libcore_symbol_prices_filter
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/core/symbol_prices_filter
)

target_link_libraries(libcore_symbol_prices_filter
  PRIVATE libcore
  PRIVATE libcore_symbols_db
  PRIVATE libcpp
  PRIVATE libnetwork
  PRIVATE libnetworkx

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE di::di
  PRIVATE fmt::fmt
  PRIVATE function2::function2
  PRIVATE magic_enum::magic_enum
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
  PRIVATE range-v3::range-v3
)