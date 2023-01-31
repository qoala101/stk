add_library(libcpp
  synchronization/cpp_lock.cc
  synchronization/cpp_mutex.cc
  timer/cpp_timer_builder.cc
  timer/cpp_timer_event_loop.cc
  timer/cpp_timer.cc
  cpp_message_exception.cc
)

add_library(vh::libcpp ALIAS libcpp)

set_target_properties(libcpp PROPERTIES
  OUTPUT_NAME cpp
)

target_include_directories(libcpp
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/cpp
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/cpp/algorithm
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/cpp/alias
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/cpp/meta
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/cpp/synchronization
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/cpp/timer
)

target_link_libraries(libcpp
  PUBLIC absl::absl
  PUBLIC thirdparty::callable
  PUBLIC function2::function2
  PUBLIC Microsoft.GSL::GSL
  PUBLIC thirdparty::NotNull
  PUBLIC polymorphic_value::polymorphic_value
)