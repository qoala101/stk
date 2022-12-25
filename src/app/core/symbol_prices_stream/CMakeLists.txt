find_package(absl REQUIRED)
find_package(cppcoro REQUIRED)
find_package(di REQUIRED)
find_package(fmt REQUIRED)
find_package(function2 REQUIRED)
find_package(magic_enum REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(polymorphic_value REQUIRED)

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
  PRIVATE libbinance
  PRIVATE libcore
  PRIVATE libcore_symbol_prices_streams_controller
  PRIVATE libcore_symbols_db
  PRIVATE libcpp
  PRIVATE libdi
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
)