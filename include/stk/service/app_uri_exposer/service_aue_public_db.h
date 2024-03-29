/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_STK_SERVICE_AUE_PUBLIC_DB_H_
#define VH_STK_SERVICE_AUE_PUBLIC_DB_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "kvdb_i_db.h"
#include "network_types.h"

namespace vh::stk::service::aue {
/**
 * @brief DB which is exposed to public.
 * Used to share the data with web clients and doesn't require
 * the app to be running.
 */
class PublicDb {
 public:
  explicit PublicDb(cpp::NnUp<kvdb::IDb> db);

  /**
   * @brief Updates URI through which the app can be accessed from outside.
   */
  auto InsertOrUpdateAppUri(network::Uri uri) const -> cppcoro::task<>;

 private:
  cpp::NnUp<kvdb::IDb> db_;
};
}  // namespace vh::stk::service::aue

#endif  // VH_STK_SERVICE_AUE_PUBLIC_DB_H_
