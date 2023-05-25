add_executable(service_buy_low_sell_high
  main.cc
  service_blsh_json_conversions.cc
)

target_include_directories(service_buy_low_sell_high
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include/stk/service/buy_low_sell_high
)

target_link_libraries(service_buy_low_sell_high
  PRIVATE
  vh::stk::libservice_symbols_db
)