find_package(Microsoft.GSL REQUIRED)
find_package(polymorphic_value REQUIRED)
find_package(di REQUIRED)
find_package(fmt REQUIRED)
find_package(function2 REQUIRED)

add_library(libcpp
  aliases/cpp_chrono.cc
  cpp_message_exception.cc
)

set_target_properties(libcpp PROPERTIES
  OUTPUT_NAME cpp
)

target_include_directories(libcpp
  PUBLIC ${PROJECT_SOURCE_DIR}/include/cpp
  PUBLIC ${PROJECT_SOURCE_DIR}/include/cpp/aliases
  PUBLIC ${PROJECT_SOURCE_DIR}/include/cpp/di
)

target_link_libraries(libcpp
  PUBLIC Microsoft.GSL::GSL
  PUBLIC NotNull
  PUBLIC polymorphic_value::polymorphic_value
  PUBLIC di::di
  PUBLIC fmt::fmt
  PUBLIC function2::function2
)