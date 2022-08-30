find_package(AWSSDK REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(range-v3 REQUIRED)
find_package(spdlog REQUIRED)

add_library(libaws
  aws_api_handle.cc
  aws_dynamodb_db.cc
  aws_dynamodb_factory.cc
)

set_target_properties(libaws PROPERTIES
  OUTPUT_NAME aws
)

target_include_directories(libaws
  PUBLIC ${PROJECT_SOURCE_DIR}/include/aws
)

target_link_libraries(libaws
  PUBLIC libnosqldb
  PRIVATE AWS::dynamodb
  PRIVATE Microsoft.GSL::GSL
  PRIVATE NotNull
  PRIVATE range-v3::range-v3
  PRIVATE spdlog::spdlog
)