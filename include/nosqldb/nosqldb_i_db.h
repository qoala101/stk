#ifndef STONKS_NOSQLDB_NOSQLDB_I_DB_H_
#define STONKS_NOSQLDB_NOSQLDB_I_DB_H_

#include "cpp_optional.h"
#include "nosqldb_types.h"

namespace stonks::nosqldb {
/**
 * @brief Generic NoSQL DB interface.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IDb {
 public:
  virtual ~IDb() = default;

  /**
   * @brief Creates table with the specified name if it doesn't exist.
   */
  virtual void CreateTableIfNotExists(const Table &table) = 0;

  /**
   * @brief Deletes the table from DB.
   */
  virtual void DropTable(const Table &table) = 0;

  /**
   * @brief Selects the item with the specified key.
   */
  [[nodiscard]] virtual auto SelectItem(const Table &table,
                                        const Key &key) const
      -> cpp::Opt<Value> = 0;

  /**
   * @brief Sets the item value for the specified key.
   */
  virtual void InsertOrUpdateItem(const Table &table, const Key &key,
                                  const Value &value) = 0;

  /**
   * @brief Deletes the item with the specified key.
   */
  virtual void DeleteItem(const Table &table, const Key &key) = 0;

  /**
   * @brief Stores this DB to the specified URI.
   */
  virtual void WriteToUri(std::string_view uri) const = 0;

 protected:
  explicit IDb() = default;
};
}  // namespace stonks::nosqldb

#endif  // STONKS_NOSQLDB_NOSQLDB_I_DB_H_
