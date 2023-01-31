#ifndef VH_TEST_STK_INJECTOR_H_
#define VH_TEST_STK_INJECTOR_H_

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
  static auto injector = vh::di::MakeInjector(
      vh::di::BindInterfaceToImplementation<vh::log::ILogger,
                                            vh::spdlog::ThreadSafeLogger>(),
      vh::di::BindInterfaceToImplementation<vh::sqldb::IDb, vh::sqlite::Db>(),
      vh::di::BindTypeToFactoryFunction<
          vh::sqlite::NativeDbHandleVariant,
          +[](const vh::sqlite::NativeDbHandlesFactory &factory) {
            return vh::sqlite::NativeDbHandleVariant{
                factory.CreateInMemoryDb()};
          }>(),
      vh::di::BindTypeToFactoryFunction<
          vh::cpp::meta::ThreadSafe<vh::cpp::NnUp<vh::sqldb::IDb>>,
          +[](vh::cpp::NnUp<vh::log::ILogger> logger,
              vh::sqlite::NativeDbHandleVariant native_db_handle) {
            vh::sqlite::NativeDbFacade::SetSynchronizationEnabled(
                native_db_handle.GetNativeDb(), true);
            auto prepared_statement_mutex_factory =
                vh::cpp::Factory<vh::cpp::MutexVariant>{
                    []() { return vh::cpp::MutexVariant{vh::cpp::Mutex{}}; },
                    {}};
            return vh::cpp::meta::AssumeThreadSafe<
                vh::cpp::NnUp<vh::sqldb::IDb>>(
                vh::cpp::MakeNnUp<vh::sqlite::Db>(
                    std::move(logger), std::move(native_db_handle),
                    std::move(prepared_statement_mutex_factory)));
          }>());
  return injector;
}
}  // namespace test::app

#endif  // VH_TEST_STK_INJECTOR_H_
