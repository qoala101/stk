find_package(fmt REQUIRED)

add_executable(symbols_db
  app_sdb_app_server.cc
  app_sdb_app.cc
  app_sdb_main.cc
)

target_include_directories(symbols_db
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/symbols_db
)

target_link_libraries(symbols_db
  PRIVATE libapp
  PRIVATE libcli
  PRIVATE librestsdk
  PRIVATE libspdlog
  PRIVATE libsqlite
  PRIVATE fmt::fmt
)