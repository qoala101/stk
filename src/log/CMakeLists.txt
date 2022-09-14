add_library(liblog INTERFACE)

target_include_directories(liblog
  INTERFACE ${PROJECT_SOURCE_DIR}/include/log
)