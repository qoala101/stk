add_library(libservice_symbols_db_updater
  service_symbols_db_updater.cc
)

set_target_properties(libservice_symbols_db_updater PROPERTIES
  OUTPUT_NAME service_symbols_db_updater
)

target_include_directories(libservice_symbols_db_updater
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/service/symbols_db_updater
)

target_link_libraries(libservice_symbols_db_updater
  PRIVATE libbinance
  PRIVATE libcore
  PRIVATE libcore_symbols_db
  PRIVATE libcore_symbols_db_updater
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE libnetwork
  PRIVATE libnetworkx
  PRIVATE libservice

  PRIVATE absl::absl
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

add_executable(service_symbols_db_updater
  main.cc
)

target_link_libraries(service_symbols_db_updater
  PRIVATE libbinance
  PRIVATE libcli
  PRIVATE libcore
  PRIVATE libcore_symbols_db
  PRIVATE libcore_symbols_db_updater
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE liblog
  PRIVATE libnetwork
  PRIVATE libnetworkx
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libservice_symbols_db
  PRIVATE libservice_symbols_db_updater
  PRIVATE libspdlog

  PRIVATE absl::absl
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