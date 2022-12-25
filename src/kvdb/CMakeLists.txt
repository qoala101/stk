add_library(libkvdb INTERFACE)

target_include_directories(libkvdb
  INTERFACE ${PROJECT_SOURCE_DIR}/include/kvdb
)