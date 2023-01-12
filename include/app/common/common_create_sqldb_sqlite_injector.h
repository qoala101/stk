#ifndef STONKS_APP_COMMON_COMMON_CREATE_SQLDB_SQLITE_INJECTOR_H_
#define STONKS_APP_COMMON_COMMON_CREATE_SQLDB_SQLITE_INJECTOR_H_

#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_other_type.h"
#include "di_enable_nn_pointers.h"
#include "di_make_injector.h"
#include "sqldb_i_db.h"
#include "sqldb_ts_db.h"
#include "sqlite_db.h"

namespace stonks::common {
inline auto CreateSqldbSqliteInjector [[nodiscard]] () {
  return di::MakeInjector(
      di::BindInterfaceToImplementation<sqldb::IDb, sqlite::Db>(),
      di::BindTypeToFactoryFunction<sqlite::SqliteDbHandle,
                                    +[](const sqlite::DbHandlesFactory &factory,
                                        const sqlite::FilePath &file_path) {
                                      return factory.LoadDbFromFileToMemory(
                                          file_path);
                                    }>(),
      di::BindTypeToOtherType<sqlite::SqliteDbHandleVariant,
                              sqlite::SqliteDbFileHandle>(),
      di::EnableNnPointers<sqldb::ts::Db>());
}
}  // namespace stonks::common

#endif  // STONKS_APP_COMMON_COMMON_CREATE_SQLDB_SQLITE_INJECTOR_H_
