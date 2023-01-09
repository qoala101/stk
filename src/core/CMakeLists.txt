add_library(libcore
  core_common.cc
)

set_target_properties(libcore PROPERTIES
  OUTPUT_NAME core
)

target_include_directories(libcore
  PUBLIC ${PROJECT_SOURCE_DIR}/include/core
)

target_link_libraries(libcore
  PRIVATE libcpp

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
)