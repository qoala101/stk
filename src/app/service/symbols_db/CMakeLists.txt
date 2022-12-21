find_package(fmt REQUIRED)

add_library(libservice_symbols_db
  service_sdb_options.cc
  service_symbols_db.cc
)

set_target_properties(libservice_symbols_db PROPERTIES
  OUTPUT_NAME service_symbols_db
)

target_include_directories(libservice_symbols_db
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/service/symbols_db
)

target_link_libraries(libservice_symbols_db
  PUBLIC libcli
  PUBLIC libcore_symbols_db
  PUBLIC libnetworkx
  PUBLIC fmt::fmt

  PRIVATE libservice
)

add_executable(service_symbols_db
  main.cc
)

target_link_libraries(service_symbols_db
  PRIVATE libnetwork
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libservice_symbols_db
  PRIVATE libsqlite
  PRIVATE libspdlog
)