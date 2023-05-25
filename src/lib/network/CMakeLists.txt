add_library(libnetwork
  auto_parsable/request_handler/network_aprh_handler_variant.cc
  auto_parsable/request_handler/network_aprh_ws_handler_variant.cc
  auto_parsable/network_auto_parsable_request_handler.cc
  auto_parsable/network_auto_parsable_request.cc
  auto_parsable/network_auto_parsable_ws_message_handler.cc
  auto_parsable/network_auto_parsable_ws_message.cc
  auto_parsable/network_auto_parsable.cc
  typed_endpoint/network_te_endpoint_types_validator_template.cc
  typed_endpoint/network_typed_endpoint_handler.cc
  typed_endpoint/network_typed_endpoint_sender.cc
  typed_endpoint/network_typed_ws_client.cc
  typed_endpoint/network_typed_ws_message_handler.cc
  typed_endpoint/network_wrong_type_exception.cc
  network_endpoint_request_dispatcher.cc
  network_json_common_conversions.cc
  network_request_exception_handler.cc
  network_response_exception_handler.cc
  network_rest_client_request_builder.cc
  network_rest_client.cc
  network_rest_request_builder.cc
  network_rest_server_builder.cc
  network_rest_server.cc
  network_ws_client_builder.cc
  network_ws_connection.cc
)
add_library(vh::libnetwork ALIAS libnetwork)

set_target_properties(libnetwork PROPERTIES
  OUTPUT_NAME network
)

target_include_directories(libnetwork
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include/lib/network
  ${PROJECT_SOURCE_DIR}/include/lib/network/auto_parsable
  ${PROJECT_SOURCE_DIR}/include/lib/network/auto_parsable/request_handler
  ${PROJECT_SOURCE_DIR}/include/lib/network/typed_endpoint
)

target_link_libraries(libnetwork
  PUBLIC
  cppcoro::cppcoro
  magic_enum::magic_enum
  vh::libcpp

  PRIVATE
  fmt::fmt
  range-v3::range-v3
)