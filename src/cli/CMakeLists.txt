find_package(CLI11 REQUIRED)
find_package(fmt REQUIRED)

add_library(libcli
  cli_app.cc
  cli_options.cc
  cli_run_scope.cc
  cli_thread_blocker.cc
  cli_wait_for_interrupt.cc
)

set_target_properties(libcli PROPERTIES
  OUTPUT_NAME cli
)

target_include_directories(libcli
  PUBLIC ${PROJECT_SOURCE_DIR}/include/cli
)

target_link_libraries(libcli
  PUBLIC libcpp

  PRIVATE CLI11::CLI11
  PRIVATE fmt::fmt
)