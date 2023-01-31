#ifndef STONKS_APP_SERVICE_INJECTOR_SERVICE_INJ_SQLDB_SQLITE_H_
#define STONKS_APP_SERVICE_INJECTOR_SERVICE_INJ_SQLDB_SQLITE_H_

#include "cpp_meta_thread_safe.h"
#include "cpp_not_null.h"
#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_factory_function.h"
#include "di_bind_type_to_other_type.h"
#include "di_make_injector.h"
#include "sqldb_i_db.h"
#include "sqlite_db.h"
#include "sqlite_native_db_facade.h"

namespace stonks::service::inj {
inline auto CreateSqldbSqliteInjector [[nodiscard]] () {
  return di::MakeInjector(
      di::BindInterfaceToImplementation<sqldb::IDb, sqlite::Db>(),
      di::BindTypeToOtherType<sqlite::NativeDbHandleVariant,
                              sqlite::NativeDbFileHandle>(),
      di::BindTypeToFactoryFunction<
          sqlite::NativeDbHandle,
          +[](const sqlite::NativeDbHandlesFactory &factory,
              const sqlite::FilePath &file_path) {
            auto native_db = factory.LoadDbFromFileToMemory(file_path);
            sqlite::NativeDbFacade::SetSynchronizationEnabled(*native_db,
                                                              false);
            return native_db;
          }>(),
      di::BindTypeToFactoryFunction<
          cpp::meta::ThreadSafe<cpp::NnUp<sqldb::IDb>>,
          +[](cpp::NnUp<log::ILogger> logger,
              sqlite::NativeDbHandleVariant native_db_handle) {
            sqlite::NativeDbFacade::SetSynchronizationEnabled(
                native_db_handle.GetNativeDb(), true);
            auto prepared_statement_mutex_factory =
                cpp::Factory<cpp::MutexVariant>{
                    []() { return cpp::MutexVariant{cpp::Mutex{}}; }, {}};
            auto db = cpp::MakeNnUp<sqlite::Db>(
                std::move(logger), std::move(native_db_handle),
                std::move(prepared_statement_mutex_factory));
            return cpp::meta::AssumeThreadSafe<cpp::NnUp<sqldb::IDb>>(
                std::move(db));
          }>());
}
}  // namespace stonks::service::inj

#endif  // STONKS_APP_SERVICE_INJECTOR_SERVICE_INJ_SQLDB_SQLITE_H_
