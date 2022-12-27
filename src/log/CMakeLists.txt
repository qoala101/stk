add_library(liblog
  reduced_logger/log_rl_log_stats.cc
  log_i_logger.cc
  log_reduced_logger.cc
)

set_target_properties(liblog PROPERTIES
  OUTPUT_NAME log
)

target_include_directories(liblog
  PUBLIC ${PROJECT_SOURCE_DIR}/include/log
  PUBLIC ${PROJECT_SOURCE_DIR}/include/log/reduced_logger
)

target_link_libraries(liblog
  PRIVATE libcpp

  PRIVATE absl::absl
  PRIVATE fmt::fmt
  PRIVATE function2::function2
  PRIVATE magic_enum::magic_enum
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
)