find_package(fmt REQUIRED)

add_executable(persistent_db_server
  app_dt_pds_app_server.cc
  app_dt_pds_app.cc
  app_dt_pds_db.cc
  app_dt_pds_main.cc
  app_dt_pds_tables.cc
)

target_include_directories(persistent_db_server
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/docker_test/common
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/docker_test/persistent_db_server
)

target_link_libraries(persistent_db_server
  PRIVATE libapp
  PRIVATE libcli
  PRIVATE libdockertestcommon
  PRIVATE librestsdk
  PRIVATE libspdlog
  PRIVATE libsqlite
  PRIVATE fmt::fmt
)