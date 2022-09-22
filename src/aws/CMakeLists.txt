find_package(AWSSDK REQUIRED)
find_package(fmt REQUIRED)
find_package(function2 REQUIRED)

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
  PUBLIC liblog
  PUBLIC libnosqldb

  PRIVATE AWS::dynamodb
  PRIVATE fmt::fmt
  PRIVATE function2::function2
)