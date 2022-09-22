find_package(CLI11 REQUIRED)

add_library(libapp
  app_wait_for_interrupt.cc
)

set_target_properties(libapp PROPERTIES
  OUTPUT_NAME app
)

target_include_directories(libapp
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app
)

target_link_libraries(libapp
  PUBLIC CLI11::CLI11
)

add_subdirectory(docker_test)
add_subdirectory(public_nosql_db)