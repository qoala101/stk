add_executable(service_symbol_prices_stream
  main.cc
)

target_link_libraries(service_symbol_prices_stream
  PRIVATE libbinance
  PRIVATE libcli
  PRIVATE libcommon
  PRIVATE libcore
  PRIVATE libcore_symbol_prices_stream
  PRIVATE libcore_symbol_prices_streams_controller
  PRIVATE libcore_symbols_db
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE liblog
  PRIVATE libnetwork
  PRIVATE libnetworkx
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libservice_symbol_prices_streams_controller
  PRIVATE libservice_symbols_db
  PRIVATE libspdlog

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE di::di
  PRIVATE function2::function2
  PRIVATE magic_enum::magic_enum
  PRIVATE Microsoft.GSL::GSL
  PRIVATE nameof::nameof
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
)