find_package(absl REQUIRED)
find_package(fmt REQUIRED)
find_package(function2 REQUIRED)
find_package(GTest REQUIRED)
find_package(magic_enum REQUIRED)
find_package(range-v3 REQUIRED)

include(GoogleTest)

enable_testing()

add_executable(unittests
  app/test_app_symbols_db.cc

  # Disabled in order to not waste AWS API credits.
  # aws/test_aws_dynamodb_db.cc
  cpp/test_cpp_move_if.cc
  cpp/test_cpp_typed_struct.cc

  restsdk/test_restsdk_client_server.cc
  restsdk/test_restsdk_json_common_conversions.cc
  restsdk/test_restsdk_json_conversions_facades.cc
  restsdk/test_restsdk_rest_request_receiver.cc
  restsdk/test_restsdk_rest_request_sender.cc
  restsdk/test_restsdk_web_socket.cc

  sqlite/test_sqlite_db.cc
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
  PRIVATE app_symbols_db
  PRIVATE libaws
  PRIVATE libcpp
  PRIVATE librestsdk
  PRIVATE libspdlog
  PRIVATE libsqlite
  PRIVATE absl::absl
  PRIVATE fmt::fmt
  PRIVATE function2::function2
  PRIVATE magic_enum::magic_enum
  PRIVATE range-v3::range-v3
)