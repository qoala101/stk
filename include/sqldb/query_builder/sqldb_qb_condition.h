#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CONDITION_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CONDITION_H_

#include <string>

namespace stonks::sqldb::qb {
/**
 * @brief API to build the condition.
 */
class Condition {
 public:
  /**
   * @param query Initial condition.
   */
  explicit Condition(std::string query);

  /**
   * @brief Adds AND-condition to the query.
   */
  [[nodiscard]] auto operator&&(const Condition &condition) -> Condition &;

  /**
   * @brief Adds OR-condition to the query.
   */
  [[nodiscard]] auto operator||(const Condition &condition) -> Condition &;

  /**
   * @brief Gives the value of the query.
   */
  [[nodiscard]] auto GetQuery() const -> const std::string &;

 private:
  std::string query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CONDITION_H_
