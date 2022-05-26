#ifndef STONKS_AWS_AWS_DYNAMO_DB_H_
#define STONKS_AWS_AWS_DYNAMO_DB_H_

#include <memory>
#include <string>

namespace stonks::aws {
/**
 * @brief Handle to AWS DynamoDB.
 * Access is defined by the user credentials specified in the current
 * environment.
 */
class DynamoDb {
 public:
  explicit DynamoDb();

  DynamoDb(const DynamoDb &) = delete;
  DynamoDb(DynamoDb &&) noexcept = default;

  auto operator=(const DynamoDb &) -> DynamoDb & = delete;
  auto operator=(DynamoDb &&) noexcept -> DynamoDb & = default;

  ~DynamoDb();

  /**
   * @brief Table which has Key and Value columns.
   */
  struct KeyValueTable {
    std::string name{};

   private:
    friend auto operator==(const KeyValueTable &, const KeyValueTable &)
        -> bool = default;
  } __attribute__((aligned(32)));  // NOLINT(*-magic-numbers)

  /**
   * @throws If action didn't succeeded.
   */
  void CreateTableIfNotExists(const KeyValueTable &table);

  struct KeyValue {
    std::string key{};
    std::string value{};
  } __attribute__((aligned(64)));  // NOLINT(*-magic-numbers)

  /**
   * @throws If action didn't succeeded.
   */
  void AddOrUpdateItem(const KeyValueTable &table, const KeyValue &key_value);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks::aws

#endif  // STONKS_AWS_AWS_DYNAMO_DB_H_
