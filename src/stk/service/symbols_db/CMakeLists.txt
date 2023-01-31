add_library(libservice_symbols_db
  service_symbols_db.cc
)

set_target_properties(libservice_symbols_db PROPERTIES
  OUTPUT_NAME service_symbols_db
)

target_include_directories(libservice_symbols_db
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/service/symbols_db
)

target_link_libraries(libservice_symbols_db
  PUBLIC libcore
  PUBLIC libservice
)

add_executable(service_symbols_db
  main.cc
)

target_link_libraries(service_symbols_db
  PRIVATE libservice_symbols_db
)