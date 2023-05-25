find_package(absl REQUIRED GLOBAL)
find_package(AWSSDK REQUIRED GLOBAL)
find_package(CLI11 REQUIRED GLOBAL)
find_package(cppcoro REQUIRED GLOBAL)
find_package(cpprestsdk REQUIRED GLOBAL)
find_package(di REQUIRED GLOBAL)
find_package(fmt REQUIRED GLOBAL)
find_package(function2 REQUIRED GLOBAL)
find_package(GTest REQUIRED GLOBAL)
find_package(magic_enum REQUIRED GLOBAL)
find_package(Microsoft.GSL REQUIRED GLOBAL)
find_package(nameof REQUIRED GLOBAL)
find_package(polymorphic_value REQUIRED GLOBAL)
find_package(range-v3 REQUIRED GLOBAL)
find_package(spdlog REQUIRED GLOBAL)
find_package(SQLite3 REQUIRED GLOBAL)

# vh: Supresses target warnings and makes includes appear in <> brackets.
function(mark_as_system_target target)
  get_target_property(target_type ${target} TYPE)

  if(${target_type} STREQUAL "INTERFACE_LIBRARY")
    set(SCOPE INTERFACE)
  else()
    set(SCOPE PUBLIC)
  endif()

  get_target_property(INCLUDE_DIRECTORIES ${target} INTERFACE_INCLUDE_DIRECTORIES)

  target_include_directories(${target}
    SYSTEM ${SCOPE}
    ${INCLUDE_DIRECTORIES}
  )
endfunction(mark_as_system_target)

add_subdirectory(callable)
add_subdirectory(notnull)