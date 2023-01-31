add_library(libkvdb INTERFACE)
add_library(vh::libkvdb ALIAS libkvdb)

target_include_directories(libkvdb
  INTERFACE ${PROJECT_SOURCE_DIR}/include/lib/kvdb
)

target_link_libraries(libkvdb
  INTERFACE vh::libcpp
  INTERFACE cppcoro::cppcoro
)