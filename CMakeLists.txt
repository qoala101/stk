cmake_minimum_required(VERSION 3.18)

# Generates compile_commands.json which should be included
# in .vscode/c_cpp_properties.json so that VS Code IntelliSence
# can find external library headeds
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  "stonks"
  VERSION 0.0.1
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# Removes the clash of U macro between cpprestsdk and gtest libs
add_compile_definitions(_TURN_OFF_PLATFORM_STRING)

add_subdirectory(src)
add_subdirectory(tests)