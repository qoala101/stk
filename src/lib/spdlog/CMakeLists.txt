add_library(libspdlog
  spdlog_thread_safe_logger.cc
)

set_target_properties(libspdlog PROPERTIES
  OUTPUT_NAME spdlog
)

target_include_directories(libspdlog
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/spdlog
)

target_link_libraries(libspdlog
  PRIVATE libcpp
  PRIVATE liblog

  PRIVATE spdlog::spdlog
)