find_package(Microsoft.GSL REQUIRED)
find_package(spdlog REQUIRED)
find_package(magic_enum REQUIRED)
find_package(range-v3 REQUIRED)
find_package(polymorphic_value REQUIRED)
find_package(Boost REQUIRED)

add_library(libnetwork
  network_endpoint_request_dispatcher.cc
  network_i_rest_request_sender.cc
  network_rest_request_builder.cc
)
set_target_properties(libnetwork PROPERTIES OUTPUT_NAME network)
target_include_directories(libnetwork PUBLIC ${PROJECT_SOURCE_DIR}/include/network)
target_include_directories(libnetwork PUBLIC ${PROJECT_SOURCE_DIR}/include/utility)
target_include_directories(libnetwork PUBLIC ${PROJECT_SOURCE_DIR}/include/json)
target_link_libraries(libnetwork PRIVATE spdlog::spdlog)
target_link_libraries(libnetwork PRIVATE range-v3::range-v3)
target_link_libraries(libnetwork PRIVATE Microsoft.GSL::GSL)
target_link_libraries(libnetwork PRIVATE Boost::boost)
target_link_libraries(libnetwork PUBLIC NotNull)
target_link_libraries(libnetwork PUBLIC polymorphic_value::polymorphic_value)