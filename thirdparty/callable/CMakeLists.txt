include(FetchContent)

FetchContent_Declare(callable SYSTEM
  GIT_REPOSITORY https://github.com/sth/callable.hpp
  GIT_TAG 079b3d7a4c5b902013bd74228c2efe15591967d3
)

FetchContent_MakeAvailable(callable)

add_library(callable INTERFACE)

target_include_directories(callable
  SYSTEM INTERFACE ${callable_SOURCE_DIR}
)