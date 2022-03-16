find_package(cpprestsdk REQUIRED)
find_package(spdlog REQUIRED)

add_library(libstonks
  get_file_service.cc
  get_currency_list_service.cc
)
set_target_properties(libstonks PROPERTIES OUTPUT_NAME stonks)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(libstonks PUBLIC cpprestsdk::cpprest)
target_link_libraries(libstonks PUBLIC spdlog::spdlog)

add_executable(stonks main.cc)
target_link_libraries(stonks PUBLIC libstonks)