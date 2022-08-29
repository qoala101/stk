add_library(libnosqldb
  empty.cc
)

set_target_properties(libnosqldb PROPERTIES
  OUTPUT_NAME nosqldb
)

target_include_directories(libnosqldb
  PUBLIC ${PROJECT_SOURCE_DIR}/include/nosqldb
)

target_link_libraries(libnosqldb
  PUBLIC libcpp
)