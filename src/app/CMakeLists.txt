add_library(libapp INTERFACE)

target_include_directories(libapp
  INTERFACE ${PROJECT_SOURCE_DIR}/include/app
)

target_link_libraries(libapp
  INTERFACE libdi
)

add_subdirectory(docker_test)
add_subdirectory(public_nosql_db)
add_subdirectory(symbol_prices_stream)