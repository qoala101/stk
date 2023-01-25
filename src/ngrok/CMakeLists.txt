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