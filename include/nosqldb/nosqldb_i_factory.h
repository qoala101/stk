#ifndef STONKS_NOSQLDB_NOSQLDB_I_FACTORY_H_
#define STONKS_NOSQLDB_NOSQLDB_I_FACTORY_H_

#include "cpp_not_null.h"
#include "nosqldb_i_db.h"

namespace stonks::nosqldb {
/**
 * @brief Abstract factory for NoSQL DB related entities.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IFactory {
 public:
  virtual ~IFactory() = default;

  /**
   * @brief Loads DB from specified URI.
   */
  [[nodiscard]] virtual auto LoadDbFromUri(std::string_view uri) const
      -> cpp::NnUp<IDb> = 0;

 protected:
  explicit IFactory() = default;
};
}  // namespace stonks::nosqldb

#endif  // STONKS_NOSQLDB_NOSQLDB_I_FACTORY_H_
