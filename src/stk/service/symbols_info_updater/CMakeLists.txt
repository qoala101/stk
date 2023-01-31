add_executable(service_symbols_info_updater
  main.cc
)

target_link_libraries(service_symbols_info_updater
  PRIVATE libservice_symbols_db
)