find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)
find_package(range-v3 REQUIRED)

add_executable(unittests
  main.cc
  binance_api.cc
  rest_request.cc
  json_conversions.cc
  finance_api.cc
  finance_conversions.cc
  finance_enum_conversions.cc
  candles_stream.cc
  buffered_candles_stream.cc
  utils.cc
  prices_stream.cc
)
target_link_libraries(unittests PRIVATE libstonks)
target_link_libraries(unittests PRIVATE spdlog::spdlog)
target_link_libraries(unittests PRIVATE GTest::gtest GTest::gtest_main)
target_link_libraries(unittests PRIVATE range-v3::range-v3)