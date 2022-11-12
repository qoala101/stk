add_library(libconnection
  app_con_sdb_app.cc
)

set_target_properties(libconnection PROPERTIES
  OUTPUT_NAME connection
)

target_include_directories(libconnection
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/connection
)

target_link_libraries(libconnection
  PUBLIC libapp
  PUBLIC libappsymbolsdb
)