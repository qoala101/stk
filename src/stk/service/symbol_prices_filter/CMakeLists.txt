add_executable(service_symbol_prices_filter
  main.cc
)

target_link_libraries(service_symbol_prices_filter
  PRIVATE vh::stk::libservice_symbols_db
  PRIVATE range-v3::range-v3
)