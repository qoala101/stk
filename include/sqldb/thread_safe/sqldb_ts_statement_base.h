#ifndef STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_STATEMENT_BASE_H_
#define STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_STATEMENT_BASE_H_

#include <mutex>

#include "cpp_not_null.h"

namespace stonks::sqldb::ts {
namespace detail {
class StatementBaseBase {
 protected:
  explicit StatementBaseBase(cpp::NnSp<std::mutex> execute_statement_mutex);

  auto GetMutex [[nodiscard]] () const -> std::mutex &;

 private:
  cpp::NnSp<std::mutex> execute_statement_mutex_;
};
}  // namespace detail

/**
 * @brief Statement wrapper which adds synchronization layer.
 */
template <typename Statement>
class StatementBase : public detail::StatementBaseBase, public Statement {
 public:
  /**
   * @param statement Statement to wrap.
   */
  StatementBase(cpp::NnUp<Statement> statement,
                cpp::NnSp<std::mutex> execute_statement_mutex)
      : detail::StatementBaseBase{std::move(execute_statement_mutex)},
        statement_{std::move(statement)} {}

 protected:
  /**
   * @brief Gives wrapped statement.
   */
  auto GetStatement [[nodiscard]] () const -> auto & { return *statement_; }

 private:
  cpp::NnUp<Statement> statement_;
};
}  // namespace stonks::sqldb::ts

#endif  // STONKS_SQLDB_THREAD_SAFE_SQLDB_TS_STATEMENT_BASE_H_
