find_package(fmt REQUIRED) # TODO(vh): Remove

add_library(libservice_symbols_db
  service_symbols_db.cc
)

set_target_properties(libservice_symbols_db PROPERTIES
  OUTPUT_NAME service_symbols_db
)

target_include_directories(libservice_symbols_db
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/service/symbols_db
)

target_link_libraries(libservice_symbols_db
  PUBLIC libcore_symbols_db
  PUBLIC libnetwork

  PRIVATE libservice
  PUBLIC callable # TODO(vh): Remove
  PUBLIC fmt::fmt # TODO(vh): Remove
)

add_executable(service_symbols_db
  main.cc
)

target_link_libraries(service_symbols_db
  PRIVATE libcli
  PRIVATE libcore_symbols_db
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libsqlite
  PRIVATE libspdlog
  PRIVATE fmt::fmt
)