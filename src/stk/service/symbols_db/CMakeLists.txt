add_library(libservice_symbols_db
  service_symbols_db.cc
)
add_library(vh::stk::libservice_symbols_db ALIAS libservice_symbols_db)

set_target_properties(libservice_symbols_db PROPERTIES
  OUTPUT_NAME service_symbols_db
)

target_include_directories(libservice_symbols_db
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/service/symbols_db
)

target_link_libraries(libservice_symbols_db
  PUBLIC vh::stk::libcore
  PUBLIC vh::stk::libservice
)

add_executable(service_symbols_db
  main.cc
)

target_link_libraries(service_symbols_db
  PRIVATE vh::stk::libservice_symbols_db
)