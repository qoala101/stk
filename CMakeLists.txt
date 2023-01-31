cmake_minimum_required(VERSION 3.25)

# Generates compile_commands.json which should be included
# in .vscode/c_cpp_properties.json so that VS Code IntelliSence
# can find external library headers.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  "stk"
  VERSION 0.2.0
  LANGUAGES CXX
)

option(THIRD_PARTY_ONLY "If true, CMake would stop after making third party dependencies available.")
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

add_subdirectory(thirdparty)

if(THIRD_PARTY_ONLY)
  return()
endif(THIRD_PARTY_ONLY)

find_program(iwyu NAMES iwyu include-what-you-use)
set(iwyu ${iwyu}
  -Xiwyu --no_comments
  -Xiwyu --no_fwd_decls
  -Xiwyu --cxx17ns
)
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu})

add_compile_options(
  -Wall
  -Wextra
  -pedantic
  -Wno-missing-braces

  # Uncomment to treat warnings like errors. Not convenient for development.
  # -Werror
  # -pedantic-errors
)

add_subdirectory(src)

if(BUILD_TESTS)
  include(CTest)
  add_subdirectory(test)
endif(BUILD_TESTS)