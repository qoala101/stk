find_package(absl REQUIRED)
find_package(cppcoro REQUIRED)
find_package(di REQUIRED)
find_package(fmt REQUIRED)
find_package(function2 REQUIRED)
find_package(magic_enum REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(nameof REQUIRED)
find_package(polymorphic_value REQUIRED)

add_library(libservice_symbols_db
  service_symbols_db.cc
)

set_target_properties(libservice_symbols_db PROPERTIES
  OUTPUT_NAME service_symbols_db
)

target_include_directories(libservice_symbols_db
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/service/symbols_db
)

target_link_libraries(libservice_symbols_db
  PRIVATE libcommon
  PRIVATE libcore
  PRIVATE libcore_symbols_db
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE libnetwork
  PRIVATE libnetworkx
  PRIVATE libservice

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE di::di
  PRIVATE fmt::fmt
  PRIVATE function2::function2
  PRIVATE magic_enum::magic_enum
  PRIVATE Microsoft.GSL::GSL
  PRIVATE nameof::nameof
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
)

add_executable(service_symbols_db
  main.cc
)

target_link_libraries(service_symbols_db
  PRIVATE libcli
  PRIVATE libcommon
  PRIVATE libcore
  PRIVATE libcore_symbols_db
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE liblog
  PRIVATE libnetwork
  PRIVATE libnetworkx
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libservice_symbols_db
  PRIVATE libsqldb
  PRIVATE libsqlite
  PRIVATE libspdlog

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE di::di
  PRIVATE fmt::fmt
  PRIVATE function2::function2
  PRIVATE magic_enum::magic_enum
  PRIVATE Microsoft.GSL::GSL
  PRIVATE nameof::nameof
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
)