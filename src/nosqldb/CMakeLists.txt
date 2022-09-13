add_library(libnosqldb INTERFACE)

target_include_directories(libnosqldb
  INTERFACE ${PROJECT_SOURCE_DIR}/include/nosqldb
)

target_link_libraries(libnosqldb
  INTERFACE libcpp
)