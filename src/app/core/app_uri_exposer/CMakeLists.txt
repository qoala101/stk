find_package(fmt REQUIRED)

add_library(libcore_app_uri_exposer
  core_app_uri_exposer.cc
  core_aue_impl.cc
  core_aue_public_db.cc
)

set_target_properties(libcore_app_uri_exposer PROPERTIES
  OUTPUT_NAME core_app_uri_exposer
)

target_include_directories(libcore_app_uri_exposer
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/core/app_uri_exposer
)

target_link_libraries(libcore_app_uri_exposer
  PUBLIC libkvdb
  PUBLIC liblog
  PUBLIC libnetwork
  PUBLIC libngrok

  PRIVATE fmt::fmt
)