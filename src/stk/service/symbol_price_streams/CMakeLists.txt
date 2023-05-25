add_executable(service_symbol_price_streams
  main.cc
)

target_link_libraries(service_symbol_price_streams
  PRIVATE
  vh::stk::libservice_symbols_db
  range-v3::range-v3
)