find_package(cpprestsdk REQUIRED)
find_package(spdlog REQUIRED)
find_package(range-v3 REQUIRED)

add_library(libstonks
  get_file_service.cc
  get_symbols_service.cc
  rest_request.cc
  binance_api.cc
  binance_settings.cc
  utils.cc
)
set_target_properties(libstonks PROPERTIES OUTPUT_NAME stonks)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(libstonks PUBLIC cpprestsdk::cpprest)
target_link_libraries(libstonks PUBLIC spdlog::spdlog)
target_link_libraries(libstonks PUBLIC range-v3::range-v3)

add_executable(stonks main.cc)
target_link_libraries(stonks PUBLIC libstonks)