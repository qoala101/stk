find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)

add_executable(unittests
  main.cc
  binance_api.cc
  rest_request.cc
  json_conversions.cc
  finance_api.cc
  finance_conversions.cc
  finance_enum_conversions.cc
  utils.cc
)
target_link_libraries(unittests PRIVATE libstonks)
target_link_libraries(unittests PRIVATE spdlog::spdlog)
target_link_libraries(unittests PRIVATE GTest::gtest GTest::gtest_main)