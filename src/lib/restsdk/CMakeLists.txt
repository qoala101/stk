add_library(librestsdk
  restsdk_json_basic_conversions.cc
  restsdk_json_native_handle.cc
  restsdk_json.cc
  restsdk_parse_json_fom_string.cc
  restsdk_rest_request_receiver.cc
  restsdk_rest_request_sender.cc
  restsdk_ws_client.cc
)
add_library(vh::librestsdk ALIAS librestsdk)

set_target_properties(librestsdk PROPERTIES
  OUTPUT_NAME restsdk
)

# Removes the clash of U macro between cpprestsdk and other libs.
target_compile_definitions(librestsdk
  PRIVATE
  _TURN_OFF_PLATFORM_STRING
)

target_include_directories(librestsdk
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include/lib/restsdk
)

target_link_libraries(librestsdk
  PUBLIC
  vh::liblog
  vh::libnetwork

  PRIVATE
  cpprestsdk::cpprest
  fmt::fmt
  range-v3::range-v3
)