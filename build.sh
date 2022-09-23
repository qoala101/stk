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

# Build Docker test app images.
# Should be rebuild when binaries image is changed.
docker compose -f ./src/app/docker_test/docker-compose.yml build

# Upload Docker test app images to Docker Hub.
docker compose -f ./src/app/docker_test/docker-compose.yml push

# run kubernetes deployment
kompose convert
# use something like all yaml files
kubectl apply -f pds-tcp-service.yaml,pds-pod.yaml,stec-tcp-service.yaml,stec-pod.yaml

# run ngrok which opens port to kubernetes

# stop deployment
kubectl delete -f pds-tcp-service.yaml,pds-pod.yaml,stec-tcp-service.yaml,stec-pod.yaml
