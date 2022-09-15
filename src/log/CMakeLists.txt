find_package(fmt REQUIRED)

add_library(liblog INTERFACE)

target_include_directories(liblog
  INTERFACE ${PROJECT_SOURCE_DIR}/include/log
)

target_link_libraries(liblog
  INTERFACE fmt::fmt
)