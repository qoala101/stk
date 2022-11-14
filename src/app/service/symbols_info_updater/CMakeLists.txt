find_package(absl REQUIRED)
find_package(fmt REQUIRED)

add_executable(service_symbols_info_updater
  main.cc
)

target_link_libraries(service_symbols_info_updater
  PRIVATE absl::absl
  PRIVATE libcli
  PRIVATE libcore
  PRIVATE libcore_symbols_db
  PRIVATE libcore_symbols_info_updater
  PRIVATE libdi
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libservice_symbols_db
  PRIVATE libspdlog
  PRIVATE fmt::fmt
)