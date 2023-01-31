add_executable(service_app_uri_exposer
  main.cc
  service_app_uri_exposer.cc
  service_aue_impl.cc
  service_aue_public_db.cc
)

target_include_directories(service_app_uri_exposer
  PUBLIC ${PROJECT_SOURCE_DIR}/include/stk/service/app_uri_exposer
)

target_link_libraries(service_app_uri_exposer
  PRIVATE libaws
  PRIVATE libcli
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE libkvdb
  PRIVATE liblog
  PRIVATE libnetwork
  PRIVATE libngrok
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libspdlog

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE di::di
  PRIVATE fmt::fmt
  PRIVATE function2::function2
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
)