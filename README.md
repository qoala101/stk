# \[WIP\] STK

[![Build And Deployment](https://github.com/qoala101/stk/actions/workflows/build-and-deployment.yml/badge.svg)](https://github.com/qoala101/stk/actions/workflows/build-and-deployment.yml)
[![Static Analysis And Tests](https://github.com/qoala101/stk/actions/workflows/static-analysis-and-tests.yml/badge.svg)](https://github.com/qoala101/stk/actions/workflows/static-analysis-and-tests.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/qoala101/stk/badge)](https://www.codefactor.io/repository/github/qoala101/stk)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## Description

STK is a collection of libraries and microservice application for financial data analysis. See [STK-UI](https://github.com/qoala101/stk-ui) for the client app.

### Libraries

- **cpp** - concepts, templates, aliases, synchronization, and other C++ utils
- **di** - extended and adapted wrapper of \[Boost::ext\].DI for dependency injection
- **cli** - command-line argument parsing, interruption handling, and lifecycle logging
- **network** - async REST, WebSocket, and JSON interfaces
- **networkx** - mapping of C++ entities to REST client-serves with TMP
- **restsdk** - **network** implementation with C++ Rest SDK
- **sqldb** - SQL DB interface and query building API with TMP
- **sqlite** - **sqldb** implementation with SQLite
- **kvdb** - async key-value DB interface
- **aws** - **kvdb** implementation as a client to AWS Dynamo DB
- **log** - multi level logger interface
- **spdlog** - **log** implementation with spdlog
- **binance** - async REST client to Binance
- **ngrok** - async REST client to ngrok

### Application

STK consists of the following services running in Docker containers deployed on the local Kubernetes cluster, which is exposed to the public via ngrok:

- **sdb** - stores market data in SQL DB
- **sps** - streams symbol prices from Binance
- **spf** - filters out insignificant price changes
- **siu** - updates market info
- **ospd** - deletes old symbol prices
- **aue** - exposes public cluster URI to clients
- [WIP] - analyzes market data

Each service is a C++ class with a coroutine API, which is wrapped in a REST client-server for interservice communication and in command-line interface for lifecycle management.

## Build

### Requirements

- Conan 1.60 - downloads C++ dependencies
- Clang 15

```sh
conan config set general.revisions_enabled=True
conan install . -if ./build --build=missing -pr=./conanprofile.txt -s build_type=Release

cmake -B ./build
cmake --build ./build --config Release
```

See **docker** images and **.github/workflows** for additional dependencies and ways to build from scratch.

## Deployment

### Requirements

- Docker
- Kubernetes

```sh
bash ./script/build_images_release.sh

kubectl apply -f ./kubernetes/app-persistent-volume.yaml
kubectl apply -f ./kubernetes/app-deployment.yaml
```

See **script/development_environment** for environment setup.

## Third-party components

### C++

- [C++ REST SDK](https://github.com/microsoft/cpprestsdk) - async REST, WebSocket, and JSON
- [CppCoro](https://github.com/lewissbaker/cppcoro) - wrapping of async code in coroutine tasks
- [range-v3](https://github.com/ericniebler/range-v3) - algorithms, transformations, views, etc.
- [SQLite](https://github.com/sqlite/sqlite) - management of file-based SQL DB
- [AWS SDK for C++](https://github.com/aws/aws-sdk-cpp) - management of Dynamo DB in the cloud
- [[Boost::ext].DI](https://github.com/boost-ext/di) - dependency injection and construction of complex objects
- [CLI11](https://github.com/CLIUtils/CLI11) - command-line arguments parsing
- [Abseil](https://github.com/abseil/abseil-cpp) - time API and flat containers
- [GSL](https://github.com/microsoft/GSL) - Expects/Ensures asserts and explicit casts
- [function2](https://github.com/Naios/function2) - movable and const-correct callbacks
- [callable.hpp](https://github.com/sth/callable.hpp) - function traits for concepts and mapping of C++ entities to REST endpoints
- [Nameof](https://github.com/Neargye/nameof) - mapping names of C++ entities to REST endpoints and SQL queries
- [not_null](https://github.com/bitwizeshift/not_null) - type-safe not-null for pointers
- [{fmt}](https://github.com/fmtlib/fmt) - string formatting and SQL query building
- [spdlog](https://github.com/gabime/spdlog) - thread-safe logging to standard output
- [Magic Enum](https://github.com/Neargye/magic_enum) - enum-string conversion
- [polymorphic_value](https://github.com/jbcoe/polymorphic_value) - "copyable" unique_ptr for JSON interface
- [GoogleTest](https://github.com/google/googletest) - unit tests

### Deployment

- [Docker](https://www.docker.com) - containers for services
- [Kubernetes](https://kubernetes.io) - deployment of containers to the local cluster
- [ngrok](https://ngrok.com) - exposing local cluster to the public
- [NGINX](https://www.nginx.com) - mapping client requests to services and CORS

## License

[MIT](https://opensource.org/license/mit)
