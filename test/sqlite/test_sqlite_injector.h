#ifndef STONKS_SQLITE_TEST_SQLITE_INJECTOR_H_
#define STONKS_SQLITE_TEST_SQLITE_INJECTOR_H_

#include <boost/di.hpp>

#include "cpp_di_enable_not_null.h"
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
  // clang-format off
  static auto injector = make_injector(
    boost::di::bind<stonks::sqlite::FilePath>().to(stonks::sqlite::FilePath{"sqlite_db_test.db"}),
    boost::di::bind<stonks::log::ILogger>().to<stonks::spdlog::Logger>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Up<stonks::log::ILogger>>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Sp<stonks::log::ILogger>>(),
    boost::di::bind<stonks::sqlite::SqliteDbHandle>().to([](const auto &injector) {
      return injector.template create<stonks::sqlite::DbHandlesFactory>().LoadDbFromFileToMemory(injector.template create<stonks::sqlite::FilePath>());
    }),
    boost::di::bind<stonks::sqlite::SqliteDbHandleVariant>().to([](const auto &injector) -> std::shared_ptr<stonks::sqlite::SqliteDbHandleVariant> {
      return std::make_shared<stonks::sqlite::SqliteDbHandleVariant>(injector.template create<stonks::sqlite::SqliteDbFileHandle>());
    })
    ,
    stonks::cpp::di::EnableNn<stonks::cpp::Sp<stonks::sqlite::SqliteDbHandleVariant>>(), 
    boost::di::bind<stonks::sqldb::IDb>().to<stonks::sqlite::Db>(),

    boost::di::bind<stonks::sqldb::IQueryBuilder>().to<stonks::sqlite::QueryBuilder>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Up<stonks::sqldb::IQueryBuilder>>()
  );
  // clang-format on
  return injector;
}
}  // namespace test::sqlite

#endif  // STONKS_SQLITE_TEST_SQLITE_INJECTOR_H_
