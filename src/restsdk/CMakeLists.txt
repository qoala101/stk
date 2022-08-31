find_package(cpprestsdk REQUIRED)
find_package(magic_enum REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(spdlog REQUIRED)

add_library(librestsdk
  restsdk_factory.cc
  restsdk_json_basic_conversions.cc
  restsdk_json_impl.cc
  restsdk_json.cc
  restsdk_rest_request_receiver.cc
  restsdk_rest_request_sender.cc
)

set_target_properties(librestsdk PROPERTIES
  OUTPUT_NAME restsdk
)

target_include_directories(librestsdk
  PUBLIC ${PROJECT_SOURCE_DIR}/include/restsdk
)

target_link_libraries(librestsdk
  PUBLIC libnetwork
  PRIVATE cpprestsdk::cpprest
  PRIVATE magic_enum::magic_enum
  PRIVATE Microsoft.GSL::GSL
  PRIVATE spdlog::spdlog
)