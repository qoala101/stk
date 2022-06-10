add_library(libany
  any.cc
)
set_target_properties(libany PROPERTIES OUTPUT_NAME any)
target_include_directories(libany PUBLIC ${PROJECT_SOURCE_DIR}/include/any)