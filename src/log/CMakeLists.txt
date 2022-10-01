add_library(liblog
  log_i_logger.cc
)

set_target_properties(liblog PROPERTIES
  OUTPUT_NAME log
)

target_include_directories(liblog
  PUBLIC ${PROJECT_SOURCE_DIR}/include/log
)