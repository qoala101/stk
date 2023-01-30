add_library(libnetworkx
  networkx_endpoint_function_traits_facade.cc
  networkx_uri.cc
  networkx_web_socket_sender.cc
)

set_target_properties(libnetworkx PROPERTIES
  OUTPUT_NAME networkx
)

target_include_directories(libnetworkx
  PUBLIC ${PROJECT_SOURCE_DIR}/include/networkx
)

target_link_libraries(libnetworkx
  PRIVATE libcpp
  PRIVATE libnetwork

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