find_package(fmt REQUIRED)

add_executable(symbol_prices_stream
  app_sps_app.cc
  app_sps_endpoints.cc
  app_sps_json_user_conversions.cc
  app_sps_main.cc
  app_sps_sdb_client.cc
)

target_include_directories(symbol_prices_stream
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/symbol_prices_stream
)

target_link_libraries(symbol_prices_stream
  PRIVATE libapp
  PRIVATE libcli
  PRIVATE librestsdk
  PRIVATE libspdlog
  PRIVATE fmt::fmt
)