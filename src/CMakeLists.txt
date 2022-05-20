find_package(cpprestsdk REQUIRED)
find_package(spdlog REQUIRED)
find_package(range-v3 REQUIRED)
find_package(magic_enum REQUIRED)
find_package(date REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(absl REQUIRED)

add_library(libstonks
  binance/binance_api.cc
  binance/binance_settings.cc
  finance_api/finance_api.cc
  finance_api/finance_conversions.cc
  finance_api/finance_enum_conversions.cc
  finance_api/finance_types.cc
  finance_db/finance_db.cc
  google_charts/google_charts_conversions.cc
  json/json_conversions.cc
  network/client.cc
  network/rest_request.cc
  network/server.cc
  network/web_socket.cc
  services/breakout_strategy_service.cc
  services/finance_db_service.cc
  services/get_candle_chart_data_service.cc
  services/get_file_service.cc
  services/get_symbols_service.cc
  services/mean_average_strategy_service.cc
  services/order_monitor_service.cc
  services/order_proxy_service.cc
  services/price_ticks_service.cc
  services_v2/finance_db_client.cc
  services_v2/finance_db_endpoints.cc
  services_v2/finance_db_server.cc
  stonks/order_proxy.cc
  stonks/order_monitor.cc
  strategies/breakout_strategy.cc
  strategies/mean_average_strategy.cc
  streams/buffered_candles_stream.cc
  streams/candles_stream.cc
  streams/price_ticks_db_stream.cc
  streams/price_ticks_stream.cc
  utility/utils.cc
)
set_target_properties(libstonks PROPERTIES OUTPUT_NAME stonks)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/binance)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/finance_api)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/finance_db)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/google_charts)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/json)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/network)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/services)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/services_v2)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/stonks)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/strategies)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/streams)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/utility)
target_link_libraries(libstonks PUBLIC cpprestsdk::cpprest)
target_link_libraries(libstonks PRIVATE spdlog::spdlog)
target_link_libraries(libstonks PRIVATE range-v3::range-v3)
target_link_libraries(libstonks PUBLIC magic_enum::magic_enum)
target_link_libraries(libstonks PRIVATE date::date)
target_link_libraries(libstonks PUBLIC Microsoft.GSL::GSL)
target_link_libraries(libstonks PRIVATE Boost::boost)
target_link_libraries(libstonks PRIVATE absl::absl)
target_link_libraries(libstonks PRIVATE libdb)

add_executable(stonks main.cc)
target_link_libraries(stonks PRIVATE libstonks)
target_link_libraries(stonks PRIVATE spdlog::spdlog)

add_executable(finance_db_server services_v2/finance_db_server.cc)
target_link_libraries(finance_db_server PRIVATE libstonks)

add_subdirectory(db)