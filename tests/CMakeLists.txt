find_package(GTest REQUIRED)
find_package(polymorphic_value REQUIRED)

include(GoogleTest)

enable_testing()

add_executable(unittests
  cpp/move_if.cc

  # restsdk/client_server.cc
  # restsdk/json_template_conversions.cc

  # restsdk/rest_request_receiver.cc
  # restsdk/rest_request_sender.cc
  # sqlite/sqlite_db.cc
  # stonks/stonks_db.cc
)

set_property(TARGET unittests
  PROPERTY CXX_INCLUDE_WHAT_YOU_USE
)

target_include_directories(unittests
  PUBLIC ${PROJECT_SOURCE_DIR}/include/cpp
  PUBLIC ${PROJECT_SOURCE_DIR}/include/cpp/aliases
)

target_link_libraries(unittests
  PRIVATE GTest::gtest GTest::gtest_main
)

# Has to come after linking to GTest and before other libs.
# Otherwise tests in third-party libs would also be discovered.
gtest_discover_tests(unittests)

target_link_libraries(unittests
  PRIVATE libcpp

  # PRIVATE libstonks
  # PRIVATE librestsdk
  # PRIVATE libsqlite
  # PRIVATE NotNull
  # PRIVATE polymorphic_value::polymorphic_value
)