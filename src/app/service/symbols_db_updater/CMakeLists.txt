add_executable(service_symbols_db_updater
  main.cc
)

target_link_libraries(service_symbols_db_updater
  PRIVATE libbinance
  PRIVATE libcli
  PRIVATE libcommon
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