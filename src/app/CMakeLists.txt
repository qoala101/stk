add_library(libapp
  app_endpoints.cc
  app_json_user_conversions.cc
)

set_target_properties(libapp PROPERTIES
  OUTPUT_NAME app
)

target_include_directories(libapp
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app
)

target_link_libraries(libapp
  PUBLIC libcore
  PUBLIC libdi
  PUBLIC libnetwork
)

add_subdirectory(docker_test)
add_subdirectory(public_nosql_db)
add_subdirectory(symbol_prices_stream)
add_subdirectory(symbols_db)
add_subdirectory(symbols_info_updater)