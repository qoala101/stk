add_library(liblog
  reduced_logger/log_rl_log_stats.cc
  log_i_logger.cc
  log_reduced_logger.cc
)

set_target_properties(liblog PROPERTIES
  OUTPUT_NAME log
)

target_include_directories(liblog
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/log
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/log/reduced_logger
)

target_link_libraries(liblog
  PUBLIC libcpp

  PRIVATE fmt::fmt
  PRIVATE magic_enum::magic_enum
)