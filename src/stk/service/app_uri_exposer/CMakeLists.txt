add_executable(service_app_uri_exposer
  main.cc
  service_app_uri_exposer.cc
  service_aue_impl.cc
  service_aue_public_db.cc
)

target_include_directories(service_app_uri_exposer
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/service/app_uri_exposer
)

target_link_libraries(service_app_uri_exposer
  PRIVATE libngrok
  PRIVATE libservice

  PRIVATE fmt::fmt
)