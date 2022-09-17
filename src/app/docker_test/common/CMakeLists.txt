add_library(libdockertestcommon
  app_dt_json_user_conversions.cc
)

set_target_properties(libdockertestcommon PROPERTIES
  OUTPUT_NAME dockertestcommon
)

target_include_directories(libdockertestcommon
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/docker_test/common
)

target_link_libraries(libdockertestcommon
  PRIVATE libnetwork
)