# stonks

## How to build

## Setup conan profile if not already
```bash
conan profile new vhcpp || true
conan profile update settings.os=Linux vhcpp
conan profile update settings.arch=x86_64 vhcpp
conan profile update settings.compiler=clang vhcpp
conan profile update settings.compiler.libcxx=libstdc++11 vhcpp
conan profile update settings.compiler.version=11 vhcpp
conan profile update env.CC=/usr/bin/clang-11 vhcpp
conan profile update env.CXX=/usr/bin/clang++-11 vhcpp

```

## Create build directory
```bash
mkdir build
cd build
```

## Install missing libraries
```bash
conan install .. --build=missing -pr=vhcpp
```

## Build
```bash
cmake ..
cmake --build .
```

## Run
```bash
../bin/stonks
```

## Run unit tests
```bash
../bin/unittests
```

## VS Code formatting settings from extensions
## Others are in .clang-format file
```json
{
  "C/C++ Include Guard.Macro Type": "Filepath",
  "C/C++ Include Guard.Path Skip": 1,
  "C/C++ Include Guard.Prefix": "STONKS_",
  "C/C++ Include Guard.Comment Style": "Line",
  "C/C++ Include Guard.Insert Blank Line": true,
  "C/C++ Include Guard.Remove Extension": false,
  "C/C++ Include Guard.Suffix": "_",
  "C/C++ Include Guard.Spaces After Endif": 2
}
```