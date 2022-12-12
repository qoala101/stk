find_package(absl REQUIRED)
find_package(fmt REQUIRED)

add_executable(service_symbols_db_updater
  main.cc
)

target_link_libraries(service_symbols_db_updater
  PRIVATE libcore_symbols_db_updater
  PRIVATE libnetwork
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libservice_symbols_db
  PRIVATE libspdlog
)