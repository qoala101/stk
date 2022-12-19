find_package(fmt REQUIRED)

add_executable(service_app_exposer
  main.cc
  service_app_uri_exposer.cc
  service_aue_impl.cc
  service_aue_public_db.cc
)

target_include_directories(service_app_exposer
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/service/app_uri_exposer
)

target_link_libraries(service_app_exposer
  PRIVATE libaws
  PRIVATE libcli
  PRIVATE libnetwork
  PRIVATE libngrok
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libspdlog
  PRIVATE fmt::fmt
)