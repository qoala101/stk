x-terminal-emulator -e ./service_buy_low_sell_high \
  --port=30001 \
  --symbols_db_host=0.0.0.0 \
  --symbols_db_port=30000 &

x-terminal-emulator -e ./service_symbol_price_streams \
  --port=30002 \
  --symbols_db_host=0.0.0.0 \
  --symbols_db_port=30000 \
  --symbols=BTCUSDT,ETHUSDT,BNBUSDT \
  --reattempt_interval=60000 &

x-terminal-emulator -e ./service_symbols_db \
  --port=30000 \
  --db_file_path=symbols_db.db &

x-terminal-emulator -e ./service_symbols_db_updater \
  --symbols_db_host=0.0.0.0 \
  --symbols_db_port=30000 \
  --update_symbols_info_interval=3600000 \
  --check_if_update_required_interval=60000 \
  --delete_old_prices_interval=3600000 \
  --keep_prices_for_duration=3600000 \
  --reattempt_interval=60000 &
