find_package(Microsoft.GSL REQUIRED)

add_library(libnetwork
  auto_parsable/network_auto_parsable_request_handler.cc
  auto_parsable/network_auto_parsable_request.cc
  auto_parsable/network_auto_parsable.cc
  typed_endpoint/network_endpoint_types_validator_template.cc
  typed_endpoint/network_typed_endpoint_handler.cc
  typed_endpoint/network_typed_endpoint_sender.cc
  typed_endpoint/network_wrong_type_exception.cc
  network_endpoint_request_dispatcher.cc
  network_json_common_conversions.cc
  network_request_exception_handler.cc
  network_response_exception_handler.cc
  network_rest_client_request_builder.cc
  network_rest_client.cc
  network_rest_request_builder.cc
  network_rest_server.cc
  network_types.cc
)

set_target_properties(libnetwork PROPERTIES
  OUTPUT_NAME network
)

target_include_directories(libnetwork
  PUBLIC ${PROJECT_SOURCE_DIR}/include/network
  PUBLIC ${PROJECT_SOURCE_DIR}/include/network/auto_parsable
  PUBLIC ${PROJECT_SOURCE_DIR}/include/network/typed_endpoint
)

target_link_libraries(libnetwork
  PUBLIC libcpp
  PUBLIC Microsoft.GSL::GSL
)