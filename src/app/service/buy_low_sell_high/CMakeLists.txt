add_executable(service_buy_low_sell_high
  main.cc
  service_blsh_json_conversions.cc
)

target_include_directories(service_buy_low_sell_high
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/service/buy_low_sell_high
)

target_link_libraries(service_buy_low_sell_high
  PRIVATE libcli
  PRIVATE libcore
  PRIVATE libcore_buy_low_sell_high
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
)