add_library(libkvdb INTERFACE)

target_include_directories(libkvdb
  INTERFACE ${PROJECT_SOURCE_DIR}/include/lib/kvdb
)

target_link_libraries(libkvdb
  INTERFACE libcpp

  INTERFACE cppcoro::cppcoro
)