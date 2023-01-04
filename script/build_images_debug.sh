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
docker build -t qoala101/stonks:service_app_uri_exposer -f ./docker/service/debug/Dockerfile . --build-arg executable_name=service_app_uri_exposer
docker build -t qoala101/stonks:service_buy_low_sell_high -f ./docker/service/debug/Dockerfile . --build-arg executable_name=service_buy_low_sell_high
docker build -t qoala101/stonks:service_symbol_price_streams -f ./docker/service/debug/Dockerfile . --build-arg executable_name=service_symbol_price_streams
docker build -t qoala101/stonks:service_symbols_db -f ./docker/service/debug/Dockerfile . --build-arg executable_name=service_symbols_db
docker build -t qoala101/stonks:service_symbols_db_updater -f ./docker/service/debug/Dockerfile . --build-arg executable_name=service_symbols_db_updater
