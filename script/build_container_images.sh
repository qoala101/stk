# Build service images.
# Should be rebuild when binaries image is changed.
docker build -t qoala101/stonks:service_app_uri_exposer -f ./src/app/service/app_uri_exposer/Dockerfile .
docker build -t qoala101/stonks:service_buy_low_sell_high -f ./src/app/service/buy_low_sell_high/Dockerfile .
docker build -t qoala101/stonks:service_symbol_price_streams -f ./src/app/service/symbol_price_streams/Dockerfile .
docker build -t qoala101/stonks:service_symbols_db -f ./src/app/service/symbols_db/Dockerfile .
docker build -t qoala101/stonks:service_symbols_db_updater -f ./src/app/service/symbols_db_updater/Dockerfile .
