add_library(libdi INTERFACE)
add_library(vh::libdi ALIAS libdi)

target_include_directories(libdi
  INTERFACE ${PROJECT_SOURCE_DIR}/include/lib/di
)

target_link_libraries(libdi
  INTERFACE vh::libcpp
  INTERFACE di::di
)