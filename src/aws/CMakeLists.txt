find_package(AWSSDK REQUIRED)
find_package(cppcoro REQUIRED)
find_package(fmt REQUIRED)
find_package(Microsoft.GSL REQUIRED)

add_library(libaws
  dynamodb/aws_dynamodb_async_db.cc
  dynamodb/aws_dynamodb_sync_db_proxy.cc
  aws_api_handle.cc
)

set_target_properties(libaws PROPERTIES
  OUTPUT_NAME aws
)

target_include_directories(libaws
  PUBLIC ${PROJECT_SOURCE_DIR}/include/aws
  PUBLIC ${PROJECT_SOURCE_DIR}/include/aws/dynamodb
)

target_link_libraries(libaws
  PRIVATE libcpp
  PRIVATE libkvdb
  PRIVATE liblog

  PRIVATE AWS::dynamodb
  PRIVATE callable
  PRIVATE cppcoro::cppcoro
  PRIVATE fmt::fmt
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
)