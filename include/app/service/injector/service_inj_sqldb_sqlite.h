#ifndef STONKS_APP_SERVICE_INJECTOR_SERVICE_INJ_SQLDB_SQLITE_H_
#define STONKS_APP_SERVICE_INJECTOR_SERVICE_INJ_SQLDB_SQLITE_H_

#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_other_type.h"
#include "di_make_injector.h"
#include "sqldb_i_db.h"
#include "sqlite_db.h"
#include "sqlite_native_db_facade.h"

namespace stonks::service::inj {
namespace detail {
template <bool kEnableSynchronization>
auto BindSqliteDbNativeDbHandle() {
  return di::BindTypeToFactoryFunction<
      sqlite::NativeDbHandle, +[](const sqlite::NativeDbHandlesFactory &factory,
                                  const sqlite::FilePath &file_path) {
        auto native_db = factory.LoadDbFromFileToMemory(file_path);
        sqlite::NativeDbFacade::SetSynchronizationEnabled(
            *native_db, kEnableSynchronization);
        return native_db;
      }>();
}
}  // namespace detail

inline auto CreateSqldbSqliteInjector [[nodiscard]] () {
  return di::MakeInjector(
      di::BindInterfaceToImplementation<sqldb::IDb, sqlite::Db>(),
      di::BindTypeToOtherType<sqlite::NativeDbHandleVariant,
                              sqlite::NativeDbFileHandle>(),
      detail::BindSqliteDbNativeDbHandle<false>());
}
}  // namespace stonks::service::inj

#endif  // STONKS_APP_SERVICE_INJECTOR_SERVICE_INJ_SQLDB_SQLITE_H_
