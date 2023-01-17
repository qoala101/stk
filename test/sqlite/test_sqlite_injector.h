#ifndef STONKS_SQLITE_TEST_SQLITE_INJECTOR_H_
#define STONKS_SQLITE_TEST_SQLITE_INJECTOR_H_

#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_other_type.h"
#include "di_bind_type_to_value.h"
#include "di_bind_value_type_to_value.h"
#include "di_make_injector.h"
#include "spdlog_logger.h"
#include "sqldb_i_db.h"
#include "sqlite_db.h"
#include "sqlite_native_db_file_handle.h"
#include "sqlite_native_db_handle_variant.h"
#include "sqlite_native_db_handles_factory.h"
#include "sqlite_native_handles.h"
#include "sqlite_types.h"

namespace test::sqlite {
inline auto Injector [[nodiscard]] () -> auto & {
  static auto injector = stonks::di::MakeInjector(
      stonks::di::BindValueTypeToValue(
          stonks::sqlite::FilePath{"sqlite_db_test.db"}),
      stonks::di::BindInterfaceToImplementation<stonks::log::ILogger,
                                                stonks::spdlog::Logger>(),
      stonks::di::BindInterfaceToImplementation<stonks::sqldb::IDb,
                                                stonks::sqlite::Db>(),
      stonks::di::BindTypeToOtherType<stonks::sqlite::NativeDbHandleVariant,
                                      stonks::sqlite::NativeDbFileHandle>(),
      stonks::di::BindTypeToFactoryFunction<
          stonks::sqlite::NativeDbHandle,
          +[](const stonks::sqlite::NativeDbHandlesFactory &factory,
              const stonks::sqlite::FilePath &file_path) {
            return factory.LoadDbFromFileToMemory(file_path);
          }>());
  return injector;
}
}  // namespace test::sqlite

#endif  // STONKS_SQLITE_TEST_SQLITE_INJECTOR_H_
