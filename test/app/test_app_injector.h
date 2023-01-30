#ifndef STONKS_APP_TEST_APP_INJECTOR_H_
#define STONKS_APP_TEST_APP_INJECTOR_H_

#include "cpp_meta_thread_safe.h"
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

namespace test::app {
inline auto Injector [[nodiscard]] () -> auto & {
  static auto injector = stonks::di::MakeInjector(
      stonks::di::BindInterfaceToImplementation<
          stonks::log::ILogger, stonks::spdlog::ThreadSafeLogger>(),
      stonks::di::BindInterfaceToImplementation<stonks::sqldb::IDb,
                                                stonks::sqlite::Db>(),
      stonks::di::BindTypeToFactoryFunction<
          stonks::sqlite::NativeDbHandleVariant,
          +[](const stonks::sqlite::NativeDbHandlesFactory &factory) {
            return stonks::sqlite::NativeDbHandleVariant{
                factory.CreateInMemoryDb()};
          }>(),
      stonks::di::BindTypeToFactoryFunction<
          stonks::cpp::meta::ThreadSafe<stonks::cpp::NnUp<stonks::sqldb::IDb>>,
          +[](stonks::cpp::NnUp<stonks::log::ILogger> logger,
              stonks::sqlite::NativeDbHandleVariant native_db_handle) {
            stonks::sqlite::NativeDbFacade::SetSynchronizationEnabled(
                native_db_handle.GetNativeDb(), true);
            auto prepared_statement_mutex_factory =
                stonks::cpp::Factory<stonks::cpp::MutexVariant>{
                    []() {
                      return stonks::cpp::MutexVariant{stonks::cpp::Mutex{}};
                    },
                    {}};
            return stonks::cpp::meta::AssumeThreadSafe<
                stonks::cpp::NnUp<stonks::sqldb::IDb>>(
                stonks::cpp::MakeNnUp<stonks::sqlite::Db>(
                    std::move(logger), std::move(native_db_handle),
                    std::move(prepared_statement_mutex_factory)));
          }>());
  return injector;
}
}  // namespace test::app

#endif  // STONKS_APP_TEST_APP_INJECTOR_H_
