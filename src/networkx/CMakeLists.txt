find_package(fmt REQUIRED)
find_package(nameof REQUIRED)

add_library(libnetworkx
  networkx_client.cc
  networkx_endpoint_function_traits_facade.cc
)

set_target_properties(libnetworkx PROPERTIES
  OUTPUT_NAME networkx
)

target_include_directories(libnetworkx
  PUBLIC ${PROJECT_SOURCE_DIR}/include/networkx
)

target_link_libraries(libnetworkx
  PUBLIC libnetwork
  PUBLIC nameof::nameof

  PRIVATE fmt::fmt
)