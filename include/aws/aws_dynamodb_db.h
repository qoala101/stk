#ifndef STONKS_AWS_AWS_DYNAMODB_DB_H_
#define STONKS_AWS_AWS_DYNAMODB_DB_H_

#include <string>
#include <string_view>

#include "cpp_smart_pointers.h"
#include "nosqldb_i_db.h"
#include "nosqldb_types.h"

namespace stonks::aws {
/**
 * @brief Handle to AWS DynamoDB.
 * Access is defined by the user credentials specified in the current
 * environment.
 */
class Db : public nosqldb::IDb {
 public:
  explicit Db();

  Db(const Db &) = delete;
  Db(Db &&) noexcept = default;

  auto operator=(const Db &) -> Db & = delete;
  auto operator=(Db &&) noexcept -> Db & = default;

  ~Db() override;

  /**
   * @brief Table which has Key and Value columns.
   */
  struct KeyValueTable {
    std::string name{};

   private:
    friend auto operator==(const KeyValueTable &, const KeyValueTable &)
        -> bool = default;
  };

  /**
   * @throws If action didn't succeeded.
   */
  void CreateTableIfNotExists(const KeyValueTable &table);

  struct KeyValue {
    std::string key{};
    std::string value{};
  };

  /**
   * @throws If action didn't succeeded.
   */
  void AddOrUpdateItem(const KeyValueTable &table, const KeyValue &key_value);

  ////////////////////////////////////////

  /**
   * @copydoc nosqldb::IDb::CreateTableIfNotExists
   */
  void CreateTableIfNotExists(const nosqldb::Table &table) override;

  /**
   * @copydoc nosqldb::IDb::DropTable
   */
  void DropTable(const nosqldb::Table &table) override;

  /**
   * @copydoc nosqldb::IDb::SelectItem
   */
  [[nodiscard]] auto SelectItem(const nosqldb::Table &table,
                                const nosqldb::Key &key) const
      -> nosqldb::Value override;

  /**
   * @copydoc nosqldb::IDb::InsertOrUpdateItem
   */
  void InsertOrUpdateItem(const nosqldb::Table &table, const nosqldb::Key &key,
                          const nosqldb::Value &value) override;

  /**
   * @copydoc nosqldb::IDb::DeleteItem
   */
  void DeleteItem(const nosqldb::Table &table,
                  const nosqldb::Key &key) override;

  /**
   * @copydoc nosqldb::IDb::WriteToUri
   */
  void WriteToUri(std::string_view uri) const override;

 private:
  class Impl;

  cpp::Up<Impl> impl_{};
};
}  // namespace stonks::aws

#endif  // STONKS_AWS_AWS_DYNAMODB_DB_H_
