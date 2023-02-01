add_library(liblog
  log_i_logger.cc
)
add_library(vh::liblog ALIAS liblog)

set_target_properties(liblog PROPERTIES
  OUTPUT_NAME log
)

target_include_directories(liblog
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/log
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/log/reduced_logger
)

target_link_libraries(liblog
  PUBLIC vh::libcpp
  PRIVATE magic_enum::magic_enum
)