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

# Disables IWYU.
set_property(TARGET unittests
  PROPERTY CXX_INCLUDE_WHAT_YOU_USE
)

target_link_libraries(unittests
  PRIVATE GTest::gtest GTest::gtest_main
)

# Has to come after linking to GTest and before other libs.
# Otherwise tests in third-party libs would also be discovered.
gtest_discover_tests(unittests)

target_link_libraries(unittests
  PRIVATE libaws
  PRIVATE libcore
  PRIVATE libcore_buy_low_sell_high
  PRIVATE libcore_symbols_db
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE libkvdb
  PRIVATE liblog
  PRIVATE libnetwork
  PRIVATE libnetworkx
  PRIVATE libngrok
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libspdlog
  PRIVATE libsqldb
  PRIVATE libsqlite

  PRIVATE absl::absl
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
  PRIVATE range-v3::range-v3
)