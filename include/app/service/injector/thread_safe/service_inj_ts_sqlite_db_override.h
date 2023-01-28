#ifndef STONKS_APP_SERVICE_INJECTOR_THREAD_SAFE_SERVICE_INJ_TS_SQLITE_DB_OVERRIDE_H_
#define STONKS_APP_SERVICE_INJECTOR_THREAD_SAFE_SERVICE_INJ_TS_SQLITE_DB_OVERRIDE_H_

#include "cpp_meta_thread_safe.h"
#include "di_bind_type_to_factory_function.h"
#include "di_override_bindings_for_type.h"
#include "service_inj_sqldb_sqlite.h"
#include "sqldb_i_db.h"

namespace stonks::service::inj::ts {
auto OverrideThreadSafeSqliteDbInjector(auto &injector) {
  return di::OverrideBindingsForType<
      cpp::meta::ThreadSafe<cpp::NnUp<sqldb::IDb>>>(
      injector, detail::BindSqliteDbNativeDbHandle<true>());
}
}  // namespace stonks::service::inj::ts

#endif  // STONKS_APP_SERVICE_INJECTOR_THREAD_SAFE_SERVICE_INJ_TS_SQLITE_DB_OVERRIDE_H_
