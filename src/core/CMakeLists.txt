add_library(libcore INTERFACE)

target_include_directories(libcore
  INTERFACE ${PROJECT_SOURCE_DIR}/include/core
)

target_link_libraries(libcore
  INTERFACE libcpp
)