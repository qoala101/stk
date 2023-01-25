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