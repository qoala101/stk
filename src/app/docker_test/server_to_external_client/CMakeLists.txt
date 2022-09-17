add_executable(server_to_external_client
  app_dt_stec_app_endpoints.cc
  app_dt_stec_app_server.cc
  app_dt_stec_app.cc
  app_dt_stec_binance_client.cc
  app_dt_stec_binance_endpoints.cc
  app_dt_stec_json_user_conversions.cc
  app_dt_stec_main.cc
)

target_include_directories(server_to_external_client
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/docker_test/common
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/docker_test/server_to_external_client
)

target_link_libraries(server_to_external_client
  PRIVATE libdockertestcommon
  PRIVATE librestsdk
  PRIVATE libspdlog
)