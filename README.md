# stonks

## How to build

## Setup conan profile if not already
conan profile new vhcpp || true
conan profile update settings.os=Linux vhcpp
conan profile update settings.arch=x86_64 vhcpp
conan profile update settings.compiler=gcc vhcpp
conan profile update settings.compiler.libcxx=libstdc++11 vhcpp
conan profile update settings.compiler.version=10 vhcpp

## Create build directory
mkdir build
cd build

## Install missing libraries
conan install .. --build=missing -pr=vhcpp

## Build
cmake ..
cmake --build .

## Run
../bin/stonks

## Set contents of .vscode/c_cpp_properties.json to following for correct IntellSence:
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