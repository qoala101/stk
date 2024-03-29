x-terminal-emulator -e ./bin/service_buy_low_sell_high \
  --port=30001 \
  --symbols_db_host=0.0.0.0 \
  --symbols_db_port=30000 &

x-terminal-emulator -e ./bin/service_old_symbol_prices_deleter \
  --symbols_db_host=0.0.0.0 \
  --symbols_db_port=30000 \
  --delete_old_prices_interval=3600000 \
  --keep_prices_for_duration=3600000 \
  --reattempt_interval=60000 &

x-terminal-emulator -e ./bin/service_symbol_price_streams \
  --symbols_db_host=0.0.0.0 \
  --symbols_db_port=30002 \
  --symbols=BTCUSDT,ETHUSDT,BNBUSDT \
  --reattempt_interval=60000 &

x-terminal-emulator -e ./bin/service_symbol_prices_filter \
  --port=30002 \
  --symbols_db_host=0.0.0.0 \
  --symbols_db_port=30000 &

x-terminal-emulator -e ./bin/service_symbols_db \
  --port=30000 \
  --db_file_path=./bin/symbols_db.db &

x-terminal-emulator -e ./bin/service_symbols_info_updater \
  --symbols_db_host=0.0.0.0 \
  --symbols_db_port=30000 \
  --update_symbols_info_interval=3600000 \
  --check_if_update_required_interval=60000 \
  --reattempt_interval=60000 &
