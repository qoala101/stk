find_package(cppcoro REQUIRED)
find_package(di REQUIRED)
find_package(fmt REQUIRED)
find_package(function2 REQUIRED)
find_package(magic_enum REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(nameof REQUIRED)
find_package(polymorphic_value REQUIRED)

add_library(libnetworkx
  networkx_client.cc
  networkx_endpoint_function_traits_facade.cc
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
  PRIVATE libdi
  PRIVATE libnetwork

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