add_executable(public_nosql_db
  app_pnd_items_interface_endpoints.cc
  app_pnd_items_interface_server.cc
  app_pnd_json_user_conversions.cc
  app_pnd_main.cc
)

target_include_directories(public_nosql_db
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/public_nosql_db
)

target_link_libraries(public_nosql_db
  PRIVATE libaws
  PRIVATE librestsdk
)