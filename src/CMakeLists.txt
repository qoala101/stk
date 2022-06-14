find_package(cpprestsdk REQUIRED)
find_package(spdlog REQUIRED)
find_package(range-v3 REQUIRED)
find_package(magic_enum REQUIRED)
find_package(date REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(absl REQUIRED)
find_package(AWSSDK REQUIRED)

add_library(libstonks
  aws/aws_api.cc
  aws/aws_dynamo_db.cc
  binance/binance_api.cc
  binance/binance_settings.cc
  clients/client_binance.cc
  clients/client_finance_db.cc
  clients/client_proxy.cc
  endpoints/endpoints_binance.cc
  endpoints/endpoints_finance_db.cc
  endpoints/endpoints_info.cc
  endpoints/endpoints_proxy.cc
  endpoints/endpoints_v1_strategy.cc
  finance_api/finance_api.cc
  finance_api/finance_conversions.cc
  finance_api/finance_enum_conversions.cc
  finance_api/finance_types.cc
  finance_db/finance_db_table_definitions.cc
  finance_db/finance_db.cc
  google_charts/google_charts_conversions.cc
  json/json_conversions.cc
  network/client.cc
  network/endpoint.cc
  network/proxy.cc
  network/rest_request.cc
  network/server.cc
  network/uri.cc
  network/web_socket.cc
  ngrok/ngrok_client.cc
  servers/server_finance_db.cc
  servers/server_info.cc
  servers/server_proxy.cc
  servers/server_v1_strategy.cc
  services/breakout_strategy_service.cc
  services/finance_db_service.cc
  services/get_candle_chart_data_service.cc
  services/get_file_service.cc
  services/get_symbols_service.cc
  services/mean_average_strategy_service.cc
  services/order_monitor_service.cc
  services/order_proxy_service.cc
  services/price_ticks_service.cc
  stonks/stonks_db_updater_price_ticks.cc
  stonks/stonks_db_updater_symbols_info.cc
  stonks/stonks_info.cc
  stonks/stonks_ngrok_aws_db_updater.cc
  stonks/stonks_order_proxy.cc
  stonks/stonks_order_monitor.cc
  strategies/breakout_strategy.cc
  strategies/mean_average_strategy.cc
  strategies/strategy.cc
  strategies/v1_strategy.cc
  streams/buffered_candles_stream.cc
  streams/candles_stream.cc
  streams/price_ticks_db_stream.cc
  streams/price_ticks_stream.cc
  streams/prices_stream.cc
  utility/utils.cc
)
set_target_properties(libstonks PROPERTIES OUTPUT_NAME stonks)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/aws)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/binance)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/clients)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/endpoints)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/finance_api)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/finance_db)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/google_charts)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/json)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/network)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/ngrok)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/servers)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/services)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/stonks)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/strategies)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/streams)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/utility)
target_link_libraries(libstonks PRIVATE libsqldb)
target_link_libraries(libstonks PRIVATE libsqlite)
target_link_libraries(libstonks PUBLIC cpprestsdk::cpprest)
target_link_libraries(libstonks PRIVATE spdlog::spdlog)
target_link_libraries(libstonks PUBLIC range-v3::range-v3)
target_link_libraries(libstonks PUBLIC magic_enum::magic_enum)
target_link_libraries(libstonks PRIVATE date::date)
target_link_libraries(libstonks PUBLIC Microsoft.GSL::GSL)
target_link_libraries(libstonks PRIVATE Boost::boost)
target_link_libraries(libstonks PRIVATE absl::absl)
target_link_libraries(libstonks PRIVATE AWS::dynamodb)

add_executable(stonks main.cc)
target_link_libraries(stonks PRIVATE libstonks)
target_link_libraries(stonks PRIVATE spdlog::spdlog)

add_executable(app_db_updater_price_ticks apps/app_db_updater_price_ticks.cc)
target_link_libraries(app_db_updater_price_ticks PRIVATE libstonks)

add_executable(app_db_updater_symbols_info apps/app_db_updater_symbols_info.cc)
target_link_libraries(app_db_updater_symbols_info PRIVATE libstonks)

add_executable(app_db apps/app_db.cc)
target_link_libraries(app_db PRIVATE libstonks)

add_executable(app_info apps/app_info.cc)
target_link_libraries(app_info PRIVATE libstonks)

add_executable(app_ngrok_aws_db_updater apps/app_ngrok_aws_db_updater.cc)
target_link_libraries(app_ngrok_aws_db_updater PRIVATE libstonks)

add_executable(app_v1_strategy apps/app_v1_strategy.cc)
target_link_libraries(app_v1_strategy PRIVATE libstonks)

add_executable(app_proxy apps/app_proxy.cc)
target_link_libraries(app_proxy PRIVATE libstonks)

add_subdirectory(sqldb)
add_subdirectory(sqlite)
add_subdirectory(network)