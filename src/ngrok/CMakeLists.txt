find_package(cppcoro REQUIRED)
find_package(di REQUIRED)
find_package(fmt REQUIRED)
find_package(function2 REQUIRED)
find_package(magic_enum REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(nameof REQUIRED)
find_package(polymorphic_value REQUIRED)

add_library(libngrok
  ngrok_api.cc
  ngrok_json_conversions.cc
)

set_target_properties(libngrok PROPERTIES
  OUTPUT_NAME ngrok
)

target_include_directories(libngrok
  PUBLIC ${PROJECT_SOURCE_DIR}/include/ngrok
)

target_link_libraries(libngrok
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE libnetwork
  PRIVATE libnetworkx

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