add_library(libcore_old_symbol_prices_deleter
  core_old_symbol_prices_deleter.cc
  core_ospd_impl.cc
)

set_target_properties(libcore_old_symbol_prices_deleter PROPERTIES
  OUTPUT_NAME core_old_symbol_prices_deleter
)

target_include_directories(libcore_old_symbol_prices_deleter
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/core/old_symbol_prices_deleter
)

target_link_libraries(libcore_old_symbol_prices_deleter
  PRIVATE libcore
  PRIVATE libcore_symbols_db
  PRIVATE libcpp

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE di::di
  PRIVATE function2::function2
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
  PRIVATE range-v3::range-v3
)