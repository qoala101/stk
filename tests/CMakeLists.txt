find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)
find_package(range-v3 REQUIRED)
find_package(magic_enum REQUIRED)

add_executable(unittests
  main.cc
  client_server.cc
)

set_property(TARGET unittests PROPERTY CXX_INCLUDE_WHAT_YOU_USE)

target_link_libraries(unittests
  PRIVATE libstonks
  PRIVATE libsqldb
  PRIVATE libsqlite
  PRIVATE libnetwork
  PRIVATE librestsdk
  PRIVATE spdlog::spdlog
  PRIVATE GTest::gtest GTest::gtest_main
  PRIVATE range-v3::range-v3
  PRIVATE magic_enum::magic_enum
)