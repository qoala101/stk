add_library(libservice
  service_client_options.cc
  service_server_options.cc
)

set_target_properties(libservice PROPERTIES
  OUTPUT_NAME service
)

target_include_directories(libservice
  PUBLIC ${PROJECT_SOURCE_DIR}/include/service
)

target_link_libraries(libservice
  PRIVATE libcli
  PRIVATE libcore
  PRIVATE libcpp
  PRIVATE libnetwork
  PRIVATE libnetworkx

  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE fmt::fmt
  PRIVATE function2::function2
  PRIVATE magic_enum::magic_enum
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
)