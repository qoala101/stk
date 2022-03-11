find_package(cpprestsdk REQUIRED)

add_library(libstonks
  get_service.cc
)
set_target_properties(libstonks PROPERTIES OUTPUT_NAME stonks)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(libstonks PUBLIC cpprestsdk::cpprest)

add_executable(stonks main.cc)
target_link_libraries(stonks PUBLIC libstonks)