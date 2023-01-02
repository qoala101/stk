cd build
cmake ..
cmake --build .
cd ..

# Build an image which should be used as base for all apps.
# Contains base OS image and packages required for networking to work.
# No need for rebuild.
docker build -t qoala101/stonks:base -f ./docker/base/Dockerfile .

# Build service images.
# Should be rebuild when binaries are rebuilt.
docker build -t qoala101/stonks:service_app_uri_exposer -f ./docker/service/debug/app_uri_exposer/Dockerfile .
docker build -t qoala101/stonks:service_buy_low_sell_high -f ./docker/service/debug/buy_low_sell_high/Dockerfile .
docker build -t qoala101/stonks:service_symbol_price_streams -f ./docker/service/debug/symbol_price_streams/Dockerfile .
docker build -t qoala101/stonks:service_symbols_db -f ./docker/service/debug/symbols_db/Dockerfile .
docker build -t qoala101/stonks:service_symbols_db_updater -f ./docker/service/debug/symbols_db_updater/Dockerfile .
