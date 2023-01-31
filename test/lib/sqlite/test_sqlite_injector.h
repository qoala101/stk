#ifndef VH_TEST_SQLITE_INJECTOR_H_
#define VH_TEST_SQLITE_INJECTOR_H_

#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_other_type.h"
#include "di_bind_type_to_value.h"
#include "di_bind_value_type_to_value.h"
#include "di_make_injector.h"
#include "spdlog_thread_safe_logger.h"
#include "sqldb_i_db.h"
#include "sqlite_db.h"
#include "sqlite_native_db_file_handle.h"
#include "sqlite_native_db_handle_variant.h"
#include "sqlite_native_db_handles_factory.h"
#include "sqlite_native_handles.h"
#include "sqlite_types.h"

namespace test::sqlite {
inline auto Injector [[nodiscard]] () -> auto & {
  static auto injector = vh::di::MakeInjector(
      vh::di::BindValueTypeToValue(vh::sqlite::FilePath{"sqlite_db_test.db"}),
      vh::di::BindInterfaceToImplementation<vh::log::ILogger,
                                            vh::spdlog::ThreadSafeLogger>(),
      vh::di::BindInterfaceToImplementation<vh::sqldb::IDb, vh::sqlite::Db>(),
      vh::di::BindTypeToOtherType<vh::sqlite::NativeDbHandleVariant,
                                  vh::sqlite::NativeDbFileHandle>(),
      vh::di::BindTypeToFactoryFunction<
          vh::sqlite::NativeDbHandle,
          +[](const vh::sqlite::NativeDbHandlesFactory &factory,
              const vh::sqlite::FilePath &file_path) {
            return factory.LoadDbFromFileToMemory(file_path);
          }>());
  return injector;
}
}  // namespace test::sqlite

#endif  // VH_TEST_SQLITE_INJECTOR_H_
