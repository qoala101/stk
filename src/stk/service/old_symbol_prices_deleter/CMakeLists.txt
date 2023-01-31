add_executable(service_old_symbol_prices_deleter
  main.cc
)

target_link_libraries(service_old_symbol_prices_deleter
  PRIVATE vh::stk::libservice_symbols_db
)