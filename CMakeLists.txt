cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  "stk"
  VERSION 0.2.1
  LANGUAGES CXX
)

option(THIRD_PARTY_ONLY "If true, CMake would stop after making third party dependencies available.")
option(FAIL_ON_WARNINGS "Whether to treat compilation warnings as errors.")
option(BUILD_TESTS "Whether to include the test folder to the build.")

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

add_compile_options(
  -Wall
  -Wextra
  -pedantic
  -Wno-missing-braces
)

if(FAIL_ON_WARNINGS)
  add_compile_options(-Werror)
endif()

find_program(CPPCHECK cppcheck)

if(CPPCHECK)
  set(CMAKE_CXX_CPPCHECK ${CPPCHECK})
  list(APPEND CMAKE_CXX_CPPCHECK
    "--enable=all"
    "--inconclusive"
    "--inline-suppr"
    "--quiet"
    "--suppress=*:${CMAKE_SOURCE_DIR}/test*"
    "--suppress=*:${FETCHCONTENT_BASE_DIR}*"
    "--suppressions-list=${CMAKE_SOURCE_DIR}/.cppcheck"
  )

  if(FAIL_ON_WARNINGS)
    list(APPEND CMAKE_CXX_CPPCHECK
      "--error-exitcode=1"
    )
  endif()
endif()

add_subdirectory(src)

if(BUILD_TESTS)
  include(CTest)
  add_subdirectory(test)
endif(BUILD_TESTS)