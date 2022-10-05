find_package(cppcoro REQUIRED)
find_package(fmt REQUIRED)

add_executable(symbols_info_updater
  app_siu_app.cc
  app_siu_binance_client.cc
  app_siu_endpoints.cc
  app_siu_json_user_conversions.cc
  app_siu_main.cc
  app_siu_sdb_app_client.cc
)

target_include_directories(symbols_info_updater
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/symbols_info_updater
)

target_link_libraries(symbols_info_updater
  PRIVATE libapp
  PRIVATE libcli
  PRIVATE librestsdk
  PRIVATE libspdlog
  PRIVATE cppcoro::cppcoro
  PRIVATE fmt::fmt
)