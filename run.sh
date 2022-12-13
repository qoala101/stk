x-terminal-emulator -e ./service_symbols_db \
  --port 6506 \
  --db_file_path symbols_db.db &

x-terminal-emulator -e ./service_symbols_db_updater \
  --update_symbols_info_interval 3600000 \
  --delete_old_prices_interval 3600000 \
  --keep_prices_for_duration 3600000 \
  --reattempt_interval 60000 \
  --symbols_db_host 0.0.0.0 \
  --symbols_db_port 6506 &

x-terminal-emulator -e ./service_symbol_prices_stream \
  --symbol BTCUSDT \
  --reattempt_interval 60000 \
  --symbols_db_host 0.0.0.0 \
  --symbols_db_port 6506 &

x-terminal-emulator -e ./service_symbol_prices_stream \
  --symbol ETHUSDT \
  --reattempt_interval 60000 \
  --symbols_db_host 0.0.0.0 \
  --symbols_db_port 6506 &

x-terminal-emulator -e ./service_buy_low_sell_high \
  --port 6507 \
  --symbols_db_host 0.0.0.0 \
  --symbols_db_port 6506 &