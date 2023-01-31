add_library(libngrok
  ngrok_api.cc
  ngrok_json_conversions.cc
)

set_target_properties(libngrok PROPERTIES
  OUTPUT_NAME ngrok
)

target_include_directories(libngrok
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/ngrok
)

target_link_libraries(libngrok
  PUBLIC libnetworkx

  PRIVATE fmt::fmt
)