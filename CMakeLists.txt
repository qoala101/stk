cmake_minimum_required(VERSION 3.18)

# Generates compile_commands.json which should be included
# in .vscode/c_cpp_properties.json so that VS Code IntelliSence
# can find external library headers.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  "stonks"
  VERSION 0.1.0
  LANGUAGES CXX
)

option(FETCH_DEPENDENCIES_ONLY "If true, CMake would stop after making dependencies declared in the main CMakeFile.txt available.")
option(BUILD_TESTS "Whether to include the test folder to the build." ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

include(FetchContent)

FetchContent_Declare(NotNull
  GIT_REPOSITORY https://github.com/bitwizeshift/not_null
  GIT_TAG c839cb1bb4cf988bd17a4c363263997275f508b8)

FetchContent_MakeAvailable(NotNull)

if(FETCH_DEPENDENCIES_ONLY)
  return()
endif(FETCH_DEPENDENCIES_ONLY)

find_program(iwyu NAMES iwyu include-what-you-use REQUIRED)
set(iwyu ${iwyu}
  -Xiwyu --no_comments
  -Xiwyu --no_fwd_decls
  -Xiwyu --cxx17ns
)
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu})

# Removes the clash of U macro between cpprestsdk and gtest libs.
add_compile_definitions(_TURN_OFF_PLATFORM_STRING)

add_compile_options(
  -Wall
  -Wextra
  -Werror
  -pedantic
  -pedantic-errors
  -Wno-missing-braces
)

add_subdirectory(src)

if(BUILD_TESTS)
  include(CTest)
  add_subdirectory(test)
endif(BUILD_TESTS)