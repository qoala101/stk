include(GoogleTest)

enable_testing()

add_executable(unittests

  # Disabled in order to not waste AWS API credits.
  # lib/aws/test_aws_dynamodb_db.cc
  lib/cpp/test_cpp_move_if.cc
  lib/cpp/test_cpp_typed_struct.cc
  lib/network/test_network_auto_parsable.cc
  lib/networkx/test_networkx_function_traits_facade.cc
  lib/restsdk/test_restsdk_client_server.cc
  lib/restsdk/test_restsdk_json_common_conversions.cc
  lib/restsdk/test_restsdk_json_conversions_facades.cc
  lib/restsdk/test_restsdk_rest_request_receiver.cc
  lib/restsdk/test_restsdk_rest_request_sender.cc
  lib/restsdk/test_restsdk_web_socket.cc
  lib/sqlite/test_sqlite_db.cc
  stk/test_stk_core_buy_low_sell_high.cc
  stk/test_stk_core_symbols_db.cc
)

target_link_libraries(unittests
  PRIVATE GTest::gtest GTest::gtest_main
)

# Has to come after linking to GTest and before other libs.
# Otherwise tests in third-party libs would also be discovered.
gtest_discover_tests(unittests)

target_link_libraries(unittests
  PRIVATE vh::stk::libcore
  PRIVATE vh::stk::libservice
  PRIVATE fmt::fmt
  PRIVATE range-v3::range-v3
)