add_library(libcore
  buy_low_sell_high/core_buy_low_sell_high.cc
  old_symbol_prices_deleter/core_old_symbol_prices_deleter.cc
  old_symbol_prices_deleter/core_ospd_impl.cc
  symbol_price_streams/core_sps_price_recorder.cc
  symbol_price_streams/core_sps_stream_factory.cc
  symbol_price_streams/core_sps_stream_handle.cc
  symbol_price_streams/core_symbol_price_streams.cc
  symbol_prices_filter/core_symbol_prices_filter.cc
  symbols_db/core_sdb_proxy.cc
  symbols_db/core_sdb_value_conversions.cc
  symbols_db/core_symbols_db.cc
  symbols_info_updater/core_siu_impl.cc
  symbols_info_updater/core_symbols_info_updater.cc
  core_common.cc
  core_json_conversions.cc
)

set_target_properties(libcore PROPERTIES
  OUTPUT_NAME core
)

target_include_directories(libcore
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/core
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/core/buy_low_sell_high
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/core/old_symbol_prices_deleter
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/core/symbol_price_streams
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/core/symbol_prices_filter
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/core/symbols_db
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/core/symbols_info_updater
)

target_link_libraries(libcore
  PUBLIC libbinance
  PUBLIC libsqldb

  PRIVATE fmt::fmt
  PRIVATE range-v3::range-v3
)