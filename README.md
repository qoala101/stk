# stonks

## How to build

## Setup conan profile if not already
```bash
conan profile new vhcpp || true
conan profile update settings.os=Linux vhcpp
conan profile update settings.arch=x86_64 vhcpp
conan profile update settings.compiler=gcc vhcpp
conan profile update settings.compiler.libcxx=libstdc++11 vhcpp
conan profile update settings.compiler.version=10 vhcpp
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

## Set contents of .vscode/c_cpp_properties.json to following for correct IntellSence
```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/gcc-10",
            "cStandard": "c17",
            "cppStandard": "c++20",
            "intelliSenseMode": "linux-gcc-x64",
            "compileCommands": "${workspaceFolder}/build/compile_commands.json"
        }
    ],
    "version": 4
}
```