include(FetchContent)

FetchContent_Declare(sqlbuilder
  GIT_REPOSITORY https://github.com/six-ddc/sql-builder
  GIT_TAG efd9bea42162a0e0ab06e7d45c537ac6a02cc10d
)

FetchContent_MakeAvailable(sqlbuilder)

add_library(sqlbuilder INTERFACE)

target_include_directories(sqlbuilder
  INTERFACE ${sqlbuilder_SOURCE_DIR}
)