find_package(function2 REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(polymorphic_value REQUIRED)

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
  PUBLIC function2::function2
  PUBLIC Microsoft.GSL::GSL
  PUBLIC NotNull
  PUBLIC polymorphic_value::polymorphic_value
)