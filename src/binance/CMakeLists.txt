find_package(cppcoro REQUIRED)
find_package(di REQUIRED)
find_package(function2 REQUIRED)
find_package(magic_enum REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(nameof REQUIRED)
find_package(polymorphic_value REQUIRED)

add_library(libbinance
  binance_api.cc
  binance_json_conversions.cc
)

set_target_properties(libbinance PROPERTIES
  OUTPUT_NAME binance
)

target_include_directories(libbinance
  PUBLIC ${PROJECT_SOURCE_DIR}/include/binance
)

target_link_libraries(libbinance
  PRIVATE libcpp
  PRIVATE libdi
  PRIVATE libnetwork
  PRIVATE libnetworkx

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