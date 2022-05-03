find_package(cpprestsdk REQUIRED)
find_package(spdlog REQUIRED)
find_package(range-v3 REQUIRED)
find_package(magic_enum REQUIRED)
find_package(date REQUIRED)
find_package(Microsoft.GSL REQUIRED)

add_library(libstonks
  utils.cc
  rest_request.cc
  json_conversions.cc
  finance_api.cc
  finance_conversions.cc
  finance_enum_conversions.cc
  finance_types.cc
  google_charts_conversions.cc
  order_proxy.cc
  candles_stream.cc
  buffered_candles_stream.cc
  breakout_strategy.cc
  order_monitor.cc
  prices_stream.cc
  mean_average_strategy.cc

  binance/binance_api.cc
  binance/binance_settings.cc

  services/get_file_service.cc
  services/get_symbols_service.cc
  services/get_candle_chart_data_service.cc
  services/breakout_strategy_service.cc
  services/order_proxy_service.cc
  services/order_monitor_service.cc
  services/mean_average_strategy_service.cc
)
set_target_properties(libstonks PROPERTIES OUTPUT_NAME stonks)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/binance)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/services)
target_link_libraries(libstonks PUBLIC cpprestsdk::cpprest)
target_link_libraries(libstonks PRIVATE spdlog::spdlog)
target_link_libraries(libstonks PRIVATE range-v3::range-v3)
target_link_libraries(libstonks PUBLIC magic_enum::magic_enum)
target_link_libraries(libstonks PRIVATE date::date)
target_link_libraries(libstonks PUBLIC Microsoft.GSL::GSL)
target_link_libraries(libstonks PRIVATE Boost::boost)

add_executable(stonks main.cc)
target_link_libraries(stonks PRIVATE libstonks)
target_link_libraries(stonks PRIVATE spdlog::spdlog)

add_subdirectory(db)