# Build an image which should be used as base for all apps.
# Contains base OS image and packages required for networking to work.
# No need for rebuild.
docker build -t qoala101/stonks:base -f ./docker/base/Dockerfile .

# Build dependencies image.
# Should be rebuilt when new packages are added to conanfile.txt or to FetchContent section of CMakeFile.txt.
docker build -t qoala101/stonks:dependencies -f ./docker/dependencies/Dockerfile .

# Build image which contains all project binaries.
# Should be rebuilt when dependencies image or any source files are changed.
docker build -t qoala101/stonks:binaries -f ./docker/binaries/Dockerfile .

# Build service images.
# Should be rebuild when binaries image is changed.
docker build -t qoala101/stonks:service_app_uri_exposer -f ./docker/service/release/Dockerfile . --build-arg executable_name=service_app_uri_exposer
docker build -t qoala101/stonks:service_buy_low_sell_high -f ./docker/service/release/Dockerfile . --build-arg executable_name=service_buy_low_sell_high
docker build -t qoala101/stonks:service_old_symbol_prices_deleter -f ./docker/service/release/Dockerfile . --build-arg executable_name=service_old_symbol_prices_deleter
docker build -t qoala101/stonks:service_symbol_price_streams -f ./docker/service/release/Dockerfile . --build-arg executable_name=service_symbol_price_streams
docker build -t qoala101/stonks:service_symbols_db -f ./docker/service/release/Dockerfile . --build-arg executable_name=service_symbols_db
docker build -t qoala101/stonks:service_symbols_info_updater -f ./docker/service/release/Dockerfile . --build-arg executable_name=service_symbols_info_updater
