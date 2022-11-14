#ifndef STONKS_APP_SERVICE_SERVICE_SQLDB_SQLITE_INJECTOR_H_
#define STONKS_APP_SERVICE_SERVICE_SQLDB_SQLITE_INJECTOR_H_

#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_other_type.h"
#include "di_make_injector.h"
#include "sqldb_i_db.h"
#include "sqlite_db.h"

namespace stonks::service::injectors {
inline auto CreateSqldbSqliteInjector [[nodiscard]] () {
  struct SqliteDbHandleFactory {
    auto operator() [[nodiscard]] (const sqlite::DbHandlesFactory &factory,
                                   const sqlite::FilePath &file_path) {
      return factory.LoadDbFromFileToMemory(file_path);
    }
  };

  return di::MakeInjector(
      di::BindInterfaceToImplementation<sqldb::IDb, sqlite::Db>(),
      di::BindTypeToFactoryFunction<
          sqlite::SqliteDbHandle, SqliteDbHandleFactory,
          sqlite::DbHandlesFactory, sqlite::FilePath>(),
      di::BindTypeToOtherType<sqlite::SqliteDbHandleVariant,
                              sqlite::SqliteDbFileHandle>());
}
}  // namespace stonks::service::injectors

#endif  // STONKS_APP_SERVICE_SERVICE_SQLDB_SQLITE_INJECTOR_H_
