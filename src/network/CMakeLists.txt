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

set_target_properties(libnetwork PROPERTIES
  OUTPUT_NAME network
)

target_include_directories(libnetwork
  PUBLIC ${PROJECT_SOURCE_DIR}/include/network
  PUBLIC ${PROJECT_SOURCE_DIR}/include/network/auto_parsable
  PUBLIC ${PROJECT_SOURCE_DIR}/include/network/auto_parsable/request_handler
  PUBLIC ${PROJECT_SOURCE_DIR}/include/network/thread_safe
  PUBLIC ${PROJECT_SOURCE_DIR}/include/network/typed_endpoint
)

target_link_libraries(libnetwork
  PRIVATE libcpp

  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE di::di
  PRIVATE fmt::fmt
  PRIVATE function2::function2
  PRIVATE magic_enum::magic_enum
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
  PRIVATE range-v3::range-v3
)