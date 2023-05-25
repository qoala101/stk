add_executable(service_symbols_info_updater
  main.cc
)

target_link_libraries(service_symbols_info_updater
  PRIVATE
  vh::stk::libservice_symbols_db
)