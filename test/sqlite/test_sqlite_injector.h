#ifndef STONKS_SQLITE_TEST_SQLITE_INJECTOR_H_
#define STONKS_SQLITE_TEST_SQLITE_INJECTOR_H_

#include "cpp_di_bind_interface_to_implementation.h"
#include "cpp_di_bind_type_to_factory_function.h"
#include "cpp_di_bind_type_to_other_type.h"
#include "cpp_di_bind_type_to_value.h"
#include "cpp_di_make_injector.h"
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
  static auto injector = stonks::cpp::di::MakeInjector(
      stonks::cpp::di::BindTypeToValue<stonks::sqlite::FilePath>(
          stonks::sqlite::FilePath{"sqlite_db_test.db"}),
      stonks::cpp::di::BindInterfaceToImplementation<stonks::log::ILogger,
                                                     stonks::spdlog::Logger>(),
      stonks::cpp::di::BindInterfaceToImplementation<stonks::sqldb::IDb,
                                                     stonks::sqlite::Db>(),
      stonks::cpp::di::BindInterfaceToImplementation<
          stonks::sqldb::IQueryBuilder, stonks::sqlite::QueryBuilder>(),
      stonks::cpp::di::BindTypeToOtherType<
          stonks::sqlite::SqliteDbHandleVariant,
          stonks::sqlite::SqliteDbFileHandle>(),
      stonks::cpp::di::BindTypeToFactoryFunction<
          stonks::sqlite::SqliteDbHandle>([](const auto &injector) {
        return injector.template create<stonks::sqlite::DbHandlesFactory>()
            .LoadDbFromFileToMemory(
                injector.template create<stonks::sqlite::FilePath>());
      }));
  return injector;
}
}  // namespace test::sqlite

#endif  // STONKS_SQLITE_TEST_SQLITE_INJECTOR_H_
