# Build an image which should be used as base for all apps.
# Contains base OS image and packages required for networking to work.
# No need for rebuild.
docker build -t qoala101/stonks:base -f ./docker/base/Dockerfile .

# Build dependencies image.
# Should be rebuilt when new packages are added to conanfile.txt or to FetchContent section of CMakeFile.txt.
docker build -t qoala101/stonks:dependencies -f ./docker/dependencies/Dockerfile .

# Build image which contains all project binaries.
# Should be rebuilt when dependencies image or any source files are changed.
docker build -t qoala101/stonks:binaries -f ./docker/binaries/release/Dockerfile .

bash ./script/build_container_images.sh
