find_package(absl REQUIRED)

add_library(libdockertestcommon
  app_dt_json_user_conversions.cc
  app_dt_pds_app_endpoints.cc
)

set_target_properties(libdockertestcommon PROPERTIES
  OUTPUT_NAME dockertestcommon
)

target_include_directories(libdockertestcommon
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/docker_test/common
)

target_link_libraries(libdockertestcommon
  PUBLIC absl::absl

  PRIVATE libapp
  PRIVATE libnetwork
)