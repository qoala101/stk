find_package(fmt REQUIRED)
find_package(range-v3 REQUIRED)

add_library(libappsymbolsdb
  app_sdb_app_server.cc
  app_sdb_app.cc
  app_sdb_prepared_statements.cc
  app_sdb_tables.cc
)

set_target_properties(libappsymbolsdb PROPERTIES
  OUTPUT_NAME appsymbolsdb
)

target_include_directories(libappsymbolsdb
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/symbols_db
)

target_link_libraries(libappsymbolsdb
  PUBLIC libapp
  PUBLIC libcli
  PUBLIC librestsdk
  PUBLIC libspdlog
  PUBLIC libsqlite
  PUBLIC range-v3::range-v3
  PUBLIC fmt::fmt
  PUBLIC sqlbuilder
)

add_executable(appsymbolsdb
  app_sdb_main.cc
)

target_link_libraries(appsymbolsdb
  PRIVATE libappsymbolsdb
)