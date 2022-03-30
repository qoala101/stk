find_package(cpprestsdk REQUIRED)
find_package(spdlog REQUIRED)
find_package(range-v3 REQUIRED)
find_package(magic_enum REQUIRED)

add_library(libstonks
  utils.cc
  rest_request.cc
  json_conversions.cc

  binance/binance_api.cc
  binance/binance_settings.cc

  services/get_file_service.cc
  services/get_symbols_service.cc
)
set_target_properties(libstonks PROPERTIES OUTPUT_NAME stonks)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/binance)
target_include_directories(libstonks PUBLIC ${PROJECT_SOURCE_DIR}/include/services)
target_link_libraries(libstonks PUBLIC cpprestsdk::cpprest)
target_link_libraries(libstonks PRIVATE spdlog::spdlog)
target_link_libraries(libstonks PRIVATE range-v3::range-v3)
target_link_libraries(libstonks PUBLIC magic_enum::magic_enum)

add_executable(stonks main.cc)
target_link_libraries(stonks PRIVATE libstonks)
target_link_libraries(stonks PRIVATE spdlog::spdlog)