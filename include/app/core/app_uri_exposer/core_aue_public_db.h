#ifndef STONKS_APP_CORE_APP_URI_EXPOSER_CORE_AUE_PUBLIC_DB_H_
#define STONKS_APP_CORE_APP_URI_EXPOSER_CORE_AUE_PUBLIC_DB_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "kvdb_i_db.h"
#include "network_types.h"

namespace stonks::core::aue {
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
}  // namespace stonks::core::aue

#endif  // STONKS_APP_CORE_APP_URI_EXPOSER_CORE_AUE_PUBLIC_DB_H_
