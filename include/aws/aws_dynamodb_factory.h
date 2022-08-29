#ifndef STONKS_AWS_AWS_DYNAMODB_FACTORY_H_
#define STONKS_AWS_AWS_DYNAMODB_FACTORY_H_

#include <string_view>

#include "cpp_not_null.h"
#include "nosqldb_i_db.h"
#include "nosqldb_i_factory.h"

namespace stonks::aws {
/**
 * @copydoc network::IFactory
 */
class Factory : public nosqldb::IFactory {
 public:
  /**
   * @copydoc nosqldb::IFactory::LoadDbFromUri
   */
  [[nodiscard]] auto LoadDbFromUri(std::string_view uri) const
      -> cpp::NnUp<nosqldb::IDb> override;
};
}  // namespace stonks::aws

#endif  // STONKS_AWS_AWS_DYNAMODB_FACTORY_H_
