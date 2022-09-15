# stonks

## Install required apps

```bash
sudo apt install cmake
sudo apt install pip
sudo pip install conan
sudo apt install snap
sudo snap install ngrok
sudo apt install iwyu
```

## AWS

Create and set AWS credentials following the guide
https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/credentials.html

## How to build

### Setup conan profile if not already

```bash
conan profile new vhcpp || true
conan profile update settings.os=Linux vhcpp
conan profile update settings.arch=x86_64 vhcpp
conan profile update settings.compiler=clang vhcpp
conan profile update settings.compiler.libcxx=libstdc++11 vhcpp
conan profile update settings.compiler.version=11 vhcpp
conan profile update settings.build_type=Debug vhcpp
conan profile update env.CC=/usr/bin/clang-11 vhcpp
conan profile update env.CXX=/usr/bin/clang++-11 vhcpp
```

### Create build directory

```bash
mkdir build
cd build
```

### Install missing libraries

```bash
conan install .. --build=missing -pr=vhcpp
```

### Build

```bash
cmake ..
cmake --build .
```

### Run

```bash
../bin/stonks
```

### Run unit tests

```bash
../bin/unittests
```

## VS Code formatting settings from extensions

## Others are in .clangd, .clang-tidy, and .clang-format files

```json
{
  "C/C++ Include Guard.Macro Type": "Filepath",
  "C/C++ Include Guard.Path Skip": 1,
  "C/C++ Include Guard.Prefix": "STONKS_",
  "C/C++ Include Guard.Comment Style": "Line",
  "C/C++ Include Guard.Insert Blank Line": true,
  "C/C++ Include Guard.Remove Extension": false,
  "C/C++ Include Guard.Suffix": "_",
  "C/C++ Include Guard.Spaces After Endif": 2,
  "iwyu.reorder": false,
  "iwyu.safe": false,
  "iwyu.no_fwd_decls": true,
  "iwyu.comments": false
}
```

### Things to do before commit

Fix includes in changed files via VS Code Include What You Use command in sources and manually in headers

# Containers

## Docker

### Installation

```bash
sudo mkdir -p /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/debian/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/debian \
  $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt update
sudo apt install docker-ce docker-ce-cli containerd.io docker-compose-plugin
```

Restart PC.

### Setup

```bash
sudo groupadd docker
sudo usermod -aG docker $USER
```

Log out and log in.

## Kubernetes

```bash
sudo curl -fsSLo /usr/share/keyrings/kubernetes-archive-keyring.gpg https://packages.cloud.google.com/apt/doc/apt-key.gpg
echo "deb [signed-by=/usr/share/keyrings/kubernetes-archive-keyring.gpg] https://apt.kubernetes.io/ kubernetes-xenial main" | sudo tee /etc/apt/sources.list.d/kubernetes.list
sudo apt update
sudo apt install -y kubectl
```

## minikube

```bash
curl -LO https://storage.googleapis.com/minikube/releases/latest/minikube_latest_amd64.deb
sudo dpkg -i minikube_latest_amd64.deb
minikube start
```