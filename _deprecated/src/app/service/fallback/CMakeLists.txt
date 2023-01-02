add_executable(service_fallback
  main.cc
  service_fallback.cc
)

target_include_directories(service_fallback
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/service/fallback
)

target_link_libraries(service_fallback
  PRIVATE libcli
  PRIVATE libcommon
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE liblog
  PRIVATE libnetwork
  PRIVATE libnetworkx
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libspdlog

  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE di::di
  PRIVATE function2::function2
  PRIVATE magic_enum::magic_enum
  PRIVATE Microsoft.GSL::GSL
  PRIVATE nameof::nameof
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
)