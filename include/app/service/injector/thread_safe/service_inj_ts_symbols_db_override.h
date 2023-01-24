#ifndef STONKS_APP_SERVICE_INJECTOR_SERVICE_OVERRIDE_THREAD_SAFE_SYMBOLS_DB_INJECTOR_H_
#define STONKS_APP_SERVICE_INJECTOR_SERVICE_OVERRIDE_THREAD_SAFE_SYMBOLS_DB_INJECTOR_H_

#include "core_i_symbols_db.h"
#include "cpp_meta_thread_safe.h"
#include "cpp_mutex.h"
#include "di_bind_type_to_other_type.h"
#include "di_override_bindings_for_type.h"

namespace stonks::service::inj::ts {
auto OverrideThreadSafeSymbolsDbInjector(auto &injector) {
  return di::OverrideBindingsForType<
      cpp::meta::ThreadSafe<cpp::NnUp<core::ISymbolsDb>>>(
      injector, di::BindTypeToOtherType<cpp::MutexVariant, cpp::Mutex>());
}
}  // namespace stonks::service::inj::ts

#endif  // STONKS_APP_SERVICE_INJECTOR_SERVICE_OVERRIDE_THREAD_SAFE_SYMBOLS_DB_INJECTOR_H_
