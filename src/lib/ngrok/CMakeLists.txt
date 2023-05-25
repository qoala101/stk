add_library(libngrok
  ngrok_api.cc
  ngrok_json_conversions.cc
)
add_library(vh::libngrok ALIAS libngrok)

set_target_properties(libngrok PROPERTIES
  OUTPUT_NAME ngrok
)

target_include_directories(libngrok
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include/lib/ngrok
)

target_link_libraries(libngrok
  PUBLIC
  vh::libnetworkx

  PRIVATE
  fmt::fmt
)