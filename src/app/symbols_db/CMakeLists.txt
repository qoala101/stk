find_package(fmt REQUIRED)
find_package(range-v3 REQUIRED)

add_executable(app_symbols_db
  app_sdb_app_server.cc
  app_sdb_app.cc
  app_sdb_main.cc
  app_sdb_prepared_statements.cc
  app_sdb_tables.cc
)

set_target_properties(app_symbols_db PROPERTIES
  ENABLE_EXPORTS ON
)

target_include_directories(app_symbols_db
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/symbols_db
)

target_link_libraries(app_symbols_db
  PRIVATE libapp
  PRIVATE libcli
  PRIVATE librestsdk
  PRIVATE libspdlog
  PRIVATE libsqlite
  PRIVATE fmt::fmt
  PRIVATE range-v3::range-v3
)