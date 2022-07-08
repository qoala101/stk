find_package(Microsoft.GSL REQUIRED)
find_package(spdlog REQUIRED)
find_package(magic_enum REQUIRED)
find_package(range-v3 REQUIRED)

add_library(librestsdk
  restsdk_factory.cc
  restsdk_rest_request_sender.cc
  restsdk_utils.cc
)
set_target_properties(librestsdk PROPERTIES OUTPUT_NAME restsdk)
target_include_directories(librestsdk PUBLIC ${PROJECT_SOURCE_DIR}/include/restsdk)
target_include_directories(librestsdk PUBLIC ${PROJECT_SOURCE_DIR}/include/network)
target_include_directories(librestsdk PUBLIC ${PROJECT_SOURCE_DIR}/include/utility)
target_include_directories(librestsdk PUBLIC ${PROJECT_SOURCE_DIR}/include/json)
target_link_libraries(librestsdk PRIVATE libnetwork)
target_link_libraries(librestsdk PRIVATE spdlog::spdlog)
target_link_libraries(librestsdk PRIVATE range-v3::range-v3)
target_link_libraries(librestsdk PUBLIC magic_enum::magic_enum)
target_link_libraries(librestsdk PUBLIC Microsoft.GSL::GSL)
target_link_libraries(librestsdk PRIVATE Boost::boost)
target_link_libraries(librestsdk PRIVATE absl::absl)
target_link_libraries(librestsdk PUBLIC NotNull)
target_link_libraries(librestsdk PRIVATE cpprestsdk::cpprest)