#ifndef STONKS_APP_APP_SQLDB_SQLITE_INJECTOR_H_
#define STONKS_APP_APP_SQLDB_SQLITE_INJECTOR_H_

#include "cpp_di_bind_interface_to_implementation.h"
#include "cpp_di_bind_type_to_factory_function.h"
#include "cpp_di_bind_type_to_other_type.h"
#include "cpp_di_make_injector.h"
#include "sqldb_i_db.h"
#include "sqldb_i_query_builder.h"
#include "sqlite_db.h"
#include "sqlite_query_builder.h"

namespace stonks::app::injectors {
[[nodiscard]] inline auto MakeSqldbSqliteInjector() {
  struct SqliteDbHandleFactory {
    [[nodiscard]] auto operator()(const sqlite::DbHandlesFactory &factory,
                                  const sqlite::FilePath &file_path)
        -> sqlite::SqliteDbHandle {
      return factory.LoadDbFromFileToMemory(file_path);
    }
  };

  return cpp::di::MakeInjector(
      cpp::di::BindInterfaceToImplementation<sqldb::IDb, sqlite::Db>(),
      cpp::di::BindInterfaceToImplementation<sqldb::IQueryBuilder,
                                             sqlite::QueryBuilder>(),
      cpp::di::BindTypeToFactoryFunction<
          sqlite::SqliteDbHandle, SqliteDbHandleFactory,
          sqlite::DbHandlesFactory, sqlite::FilePath>(),
      cpp::di::BindTypeToOtherType<sqlite::SqliteDbHandleVariant,
                                   sqlite::SqliteDbFileHandle>());
}
}  // namespace stonks::app::injectors

#endif  // STONKS_APP_APP_SQLDB_SQLITE_INJECTOR_H_
