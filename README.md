# \[WIP\] STK

[![CI](https://github.com/qoala101/stk/actions/workflows/ci.yml/badge.svg)](https://github.com/qoala101/stk/actions/workflows/ci.yml)
[![Tests](https://github.com/qoala101/stk/actions/workflows/tests.yml/badge.svg)](https://github.com/qoala101/stk/actions/workflows/tests.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/qoala101/stk/badge)](https://www.codefactor.io/repository/github/qoala101/stk)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## Description

STK is a collection of libraries and microservice application for financial data analysis built on top of it.  
See [stk_ui](https://github.com/qoala101/stk_ui) for the client app.

## Libraries

- **aws** - async client to key-value AWS Dynamo DB in the cloud
- **binance** - async REST client to Binance
- **cli** - command line argument parsing, interruption handling, and lifecycle logging for services
- **cpp** - concepts, templates, aliases, synchronization primitives, and other C++ utils
- **di** - extended and adapted wrapper of \[Boost::ext\].DI for dependency injection
- **kvdb** - async key-value DB interface with table and item management
- **log** - multi level logger interface
- **network** - async REST and WebSocket client-server and JSON interfaces with parameter validation, exception propagation, and template-based conversions
- **networkx** - high level TMP mapping of C++ types and functions to REST and WebSocket client-servers
- **ngrok** - async REST client to the local ngrok instance
- **restsdk** - async implementation of network and JSON interfaces with C++ Rest SDK
- **spdlog** - thread-safe implementation of the logging interface with spdlog
- **sqldb** - SQL DB management interface and extensive query building API with prepared statements and TMP mapping of C++ types to DB entities
- **sqlite** - implementation of the SQL DB interface with file-based SQLite

## Application

### Deployment

STK consists of several services running in Docker containers deployed on the local Kubernetes cluster.
ngrok instance running in the same cluster exposes the local cluster port to the public.
Once exposed, the public cluster URI is written to AWS Dynamo DB from where clients can get it to access the app.
Access to the service's endpoints and CORS is managed by NGINX ingress.
Persistent data is stored in Docker volumes.

### C++

Each service core functionality is C++ interface with coroutine functions.
On the server side, implementation is exposed with a REST server, which maps incoming requests to C++ functions.
On the client side, the interface is implemented as a REST client, which maps C++ functions to server endpoints.
Client-server building, JSON-C++ conversions, parameter validation, exception propagation, and fail reattempts are automatically handled with TMP and function traits.

Services are build as command line applications with argument parsing, interruption handling, and logging.
They are constructed with **di** which automatically injects command line arguments and library implementations.

All async code is wrapped in cppcoro::tasks and handled as coroutines.
Most of the libraries are implemented with extensive use of concepts and TMP.

### Services

- **Symbols DB** - interface to file-based SQL DB which stores financial data
- **Symbols Info Updated** - periodically updates the Symbols DB with market state like trading amounts or price steps
- **Symbol Prices Streams** - continually streams ever changing symbol prices to Symbols DB
- **Symbol Prices Filter** - filters out duplicated and unimportant symbol prices before they get to DB to reduce the load
- **Old Symbol Prices Deleter** - periodically deletes outdated symbol prices to reduce DB size
- **App URI Exposer** - periodically writes public cluster URI from ngrok to AWS Dynamo DB
- \[WIP\] Analytical Services

## Build

Conan is used to download and build most of C++ dependencies.

```sh
conan config set general.revisions_enabled=True
conan install . -if ./build --build=missing -pr=./conanprofile.txt -s build_type=Release

cmake -B ./build
cmake --build ./build --config Release
```

See **docker** images and **.github/workflows** for additional dependencies and ways to build from scratch.

## Deployment

Deployment requires Docker and Kubernetes.

```sh
bash ./script/build_images_release.sh
kubectl apply -f ./kubernetes/app-persistent-volume.yaml
kubectl apply -f ./kubernetes/app-deployment.yaml
```

See **script/development_environment** for environment setup.

## Third-party components

### C++

- [C++ REST SDK](https://github.com/microsoft/cpprestsdk) - async REST and WebSocket client/server and JSON
- [CppCoro](https://github.com/lewissbaker/cppcoro) - wrapping of async code in coroutine tasks
- [range-v3](https://github.com/ericniebler/range-v3) - algorithms, transformations, views, etc.
- [SQLite](https://github.com/sqlite/sqlite) - management of file-based SQL DB
- [AWS SDK for C++](https://github.com/aws/aws-sdk-cpp) - management of key-value Dynamo DB in the cloud
- [[Boost::ext].DI](https://github.com/boost-ext/di) - dependency injection and construction of complex objects
- [CLI11](https://github.com/CLIUtils/CLI11) - command line arguments parsing
- [Abseil](https://github.com/abseil/abseil-cpp) - time API and flat containers
- [GSL](https://github.com/microsoft/GSL) - Expects/Ensures asserts and explicit casts
- [function2](https://github.com/Naios/function2) - movable and const-correct callbacks
- [callable.hpp](https://github.com/sth/callable.hpp) - function traits for concepts and mapping of C++ functions to REST endpoints
- [Nameof](https://github.com/Neargye/nameof) - mapping names of C++ types and functions to REST endpoints and SQL queries
- [not_null](https://github.com/bitwizeshift/not_null) - type-safe not-null for raw and smart pointers
- [{fmt}](https://github.com/fmtlib/fmt) - string formatting and SQL query building
- [spdlog](https://github.com/gabime/spdlog) - thread-safe logging to standard output
- [Magic Enum](https://github.com/Neargye/magic_enum) - enum-string conversion
- [polymorphic_value](https://github.com/jbcoe/polymorphic_value) - "copyable" unique_ptr for JSON interface
- [GoogleTest](https://github.com/google/googletest) - unit tests

### Deployment

- [Docker](https://www.docker.com/) - containers for services
- [Kubernetes](https://kubernetes.io/) - deployment of containers to the local cluster
- [ngrok](https://ngrok.com/) - exposing local cluster to the public
- [NGINX](https://www.nginx.com/) - mapping client requests to services and CORS

## License

[MIT](https://opensource.org/license/mit/)
