#ifndef STONKS_APP_TEST_APP_INJECTOR_H_
#define STONKS_APP_TEST_APP_INJECTOR_H_

#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_other_type.h"
#include "di_bind_type_to_value.h"
#include "di_bind_value_type_to_value.h"
#include "di_make_injector.h"
#include "spdlog_logger.h"
#include "sqldb_i_db.h"
#include "sqlite_db.h"
#include "sqlite_db_file_handle.h"
#include "sqlite_db_handle_variant.h"
#include "sqlite_db_handles_factory.h"
#include "sqlite_raw_handles.h"
#include "sqlite_types.h"

namespace test::app {
inline auto Injector [[nodiscard]] () -> auto & {
  static auto injector = stonks::di::MakeInjector(
      stonks::di::BindInterfaceToImplementation<stonks::log::ILogger,
                                                stonks::spdlog::Logger>(),
      stonks::di::BindInterfaceToImplementation<stonks::sqldb::IDb,
                                                stonks::sqlite::Db>(),
      stonks::di::BindTypeToFactoryFunction<
          stonks::sqlite::SqliteDbHandleVariant,
          +[](const stonks::sqlite::DbHandlesFactory &factory) {
            return stonks::sqlite::SqliteDbHandleVariant{
                factory.CreateInMemoryDb()};
          }>());
  return injector;
}
}  // namespace test::app

#endif  // STONKS_APP_TEST_APP_INJECTOR_H_
