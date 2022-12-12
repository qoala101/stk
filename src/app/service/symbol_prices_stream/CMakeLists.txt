add_executable(service_symbol_prices_stream
  main.cc
)

target_link_libraries(service_symbol_prices_stream
  PRIVATE libcore_symbol_prices_stream
  PRIVATE libnetwork
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libservice_symbols_db
  PRIVATE libspdlog
)