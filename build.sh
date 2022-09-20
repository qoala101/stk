# if conanfile.txt changed, build stonks-deps image

# build app images
docker compose build

# upload app images to docker hub
docker tag c2ec5e59b2f4 qoala101/stonks:stonks-stec
docker tag 2d60754852e4 qoala101/stonks:stonks-pds
docker push qoala101/stonks:stonks-stec
docker push qoala101/stonks:stonks-pds

# run kubernetes deployment
kompose convert
kubectl apply -f pds-tcp-service.yaml,pds-pod.yaml,stec-tcp-service.yaml,stec-pod.yaml

# run ngrok which opens port to kubernetes

# stop deployment
kubectl delete -f pds-tcp-service.yaml,pds-pod.yaml,stec-tcp-service.yaml,stec-pod.yaml