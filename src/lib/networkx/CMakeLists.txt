add_library(libnetworkx
  networkx_endpoint_function_traits_facade.cc
  networkx_uri.cc
  networkx_web_socket_sender.cc
)
add_library(vh::libnetworkx ALIAS libnetworkx)

set_target_properties(libnetworkx PROPERTIES
  OUTPUT_NAME networkx
)

target_include_directories(libnetworkx
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/networkx
)

target_link_libraries(libnetworkx
  PUBLIC nameof::nameof
  PUBLIC vh::libnetwork
  PRIVATE fmt::fmt
)