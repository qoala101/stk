add_executable(service_symbol_price_streams
  main.cc
)

target_link_libraries(service_symbol_price_streams
  PRIVATE libservice_symbols_db

  PRIVATE range-v3::range-v3
)