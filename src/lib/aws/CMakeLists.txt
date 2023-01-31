add_library(libaws
  dynamodb/aws_dynamodb_async_db.cc
  dynamodb/aws_dynamodb_sync_db_proxy.cc
  aws_api_handle.cc
)
add_library(vh::libaws ALIAS libaws)

set_target_properties(libaws PROPERTIES
  OUTPUT_NAME aws
)

target_include_directories(libaws
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/aws
  PUBLIC ${PROJECT_SOURCE_DIR}/include/lib/aws/dynamodb
)

target_link_libraries(libaws
  PUBLIC vh::libkvdb
  PUBLIC vh::liblog
  PRIVATE AWS::dynamodb
  PRIVATE fmt::fmt
)