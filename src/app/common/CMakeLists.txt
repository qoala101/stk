add_library(libcommon
  common_json_conversions.cc
)

set_target_properties(libcommon PROPERTIES
  OUTPUT_NAME common
)

target_include_directories(libcommon
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/common
)

target_link_libraries(libcommon
  PRIVATE libcore
  PRIVATE libcpp
  PRIVATE libnetwork

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE magic_enum::magic_enum
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
)