add_executable(service_symbol_price_streams
  main.cc
)

target_link_libraries(service_symbol_price_streams
  PRIVATE libbinance
  PRIVATE libcli
  PRIVATE libcore
  PRIVATE libcore_symbol_price_streams
  PRIVATE libcore_symbols_db
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE liblog
  PRIVATE libnetwork
  PRIVATE libnetworkx
  PRIVATE librestsdk
  PRIVATE libservice
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
  PRIVATE range-v3::range-v3
)