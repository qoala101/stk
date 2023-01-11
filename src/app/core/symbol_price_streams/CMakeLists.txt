add_library(libcore_symbol_price_streams
  core_sps_book_tick_handler.cc
  core_sps_book_tick_web_socket_factory.cc
  core_sps_stream_handle.cc
  core_symbol_price_streams.cc
)

set_target_properties(libcore_symbol_price_streams PROPERTIES
  OUTPUT_NAME core_symbol_price_streams
)

target_include_directories(libcore_symbol_price_streams
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/core/symbol_price_streams
)

target_link_libraries(libcore_symbol_price_streams
  PRIVATE libbinance
  PRIVATE libcore
  PRIVATE libcore_symbols_db
  PRIVATE libcore_symbols_db_updater
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
  PRIVATE range-v3::range-v3
)