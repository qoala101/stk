find_package(di REQUIRED)

add_library(libdi INTERFACE)

target_include_directories(libdi
  INTERFACE ${PROJECT_SOURCE_DIR}/include/di
)

target_link_libraries(libdi
  INTERFACE libcpp
  INTERFACE di::di
)