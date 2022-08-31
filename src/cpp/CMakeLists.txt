add_library(libcpp
  cpp_message_exception.cc
)

set_target_properties(libcpp PROPERTIES
  OUTPUT_NAME cpp
)

target_include_directories(libcpp
  PUBLIC ${PROJECT_SOURCE_DIR}/include/cpp
  PUBLIC ${PROJECT_SOURCE_DIR}/include/cpp/aliases
)

target_link_libraries(libcpp
  PUBLIC NotNull
)