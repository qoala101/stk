#include "aws_dynamodb_factory.h"

#include "aws_dynamodb_db.h"
#include "cpp_not_null.h"

namespace stonks::aws {
auto Factory::LoadDbFromUri(std::string_view uri) const
    -> cpp::NnUp<nosqldb::IDb> {
  return cpp::MakeNnUp<Db>();
}
}  // namespace stonks::aws