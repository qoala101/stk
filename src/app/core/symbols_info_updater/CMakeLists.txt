add_library(libcore_symbols_info_updater
  core_siu_impl.cc
  core_symbols_info_updater.cc
)

set_target_properties(libcore_symbols_info_updater PROPERTIES
  OUTPUT_NAME core_symbols_info_updater
)

target_include_directories(libcore_symbols_info_updater
  PUBLIC ${PROJECT_SOURCE_DIR}/include/app/core/symbols_info_updater
)

target_link_libraries(libcore_symbols_info_updater
  PRIVATE libbinance
  PRIVATE libcore
  PRIVATE libcore_symbols_db
  PRIVATE libcpp
  PRIVATE libnetwork

  PRIVATE absl::absl
  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE di::di
  PRIVATE function2::function2
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
  PRIVATE polymorphic_value::polymorphic_value
  PRIVATE range-v3::range-v3
)