#ifndef STONKS_SQLITE_TEST_SQLITE_INJECTOR_H_
#define STONKS_SQLITE_TEST_SQLITE_INJECTOR_H_

#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_other_type.h"
#include "di_bind_type_to_value.h"
#include "di_make_injector.h"
#include "spdlog_logger.h"
#include "sqldb_i_db.h"
#include "sqldb_i_query_builder.h"
#include "sqlite_db.h"
#include "sqlite_db_file_handle.h"
#include "sqlite_db_handle_variant.h"
#include "sqlite_db_handles_factory.h"
#include "sqlite_query_builder.h"
#include "sqlite_raw_handles.h"
#include "sqlite_types.h"

namespace test::sqlite {
[[nodiscard]] inline auto Injector() -> auto & {
  struct SqliteDbHandleFactory {
    [[nodiscard]] auto operator()(
        const stonks::sqlite::DbHandlesFactory &factory,
        const stonks::sqlite::FilePath &file_path)
        -> stonks::sqlite::SqliteDbHandle {
      return factory.LoadDbFromFileToMemory(file_path);
    }
  };

  static auto injector = stonks::di::MakeInjector(
      stonks::di::BindTypeToValue<stonks::sqlite::FilePath>(
          stonks::sqlite::FilePath{"sqlite_db_test.db"}),
      stonks::di::BindInterfaceToImplementation<stonks::log::ILogger,
                                                stonks::spdlog::Logger>(),
      stonks::di::BindInterfaceToImplementation<stonks::sqldb::IDb,
                                                stonks::sqlite::Db>(),
      stonks::di::BindInterfaceToImplementation<stonks::sqldb::IQueryBuilder,
                                                stonks::sqlite::QueryBuilder>(),
      stonks::di::BindTypeToOtherType<stonks::sqlite::SqliteDbHandleVariant,
                                      stonks::sqlite::SqliteDbFileHandle>(),
      stonks::di::BindTypeToFactoryFunction<
          stonks::sqlite::SqliteDbHandle, SqliteDbHandleFactory,
          stonks::sqlite::DbHandlesFactory, stonks::sqlite::FilePath>());
  return injector;
}
}  // namespace test::sqlite

#endif  // STONKS_SQLITE_TEST_SQLITE_INJECTOR_H_
