add_library(libservice_symbol_prices_streams_controller
  service_symbol_prices_streams_controller.cc
)

set_target_properties(libservice_symbol_prices_streams_controller PROPERTIES
  OUTPUT_NAME service_symbol_prices_streams_controller
)

target_include_directories(libservice_symbol_prices_streams_controller
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/service/symbol_prices_streams_controller
)

target_link_libraries(libservice_symbol_prices_streams_controller
  PRIVATE libcli
  PRIVATE libcommon
  PRIVATE libcore
  PRIVATE libcore_symbol_prices_streams_controller
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE libnetwork
  PRIVATE libnetworkx
  PRIVATE libservice

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE di::di
  PRIVATE fmt::fmt
  PRIVATE function2::function2
  PRIVATE magic_enum::magic_enum
  PRIVATE Microsoft.GSL::GSL
  PRIVATE nameof::nameof
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
)

add_executable(service_symbol_prices_streams_controller
  main.cc
)

target_link_libraries(service_symbol_prices_streams_controller
  PRIVATE libcli
  PRIVATE libcommon
  PRIVATE libcore
  PRIVATE libcore_symbol_prices_streams_controller
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE liblog
  PRIVATE libnetwork
  PRIVATE libnetworkx
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libservice_symbol_prices_streams_controller
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