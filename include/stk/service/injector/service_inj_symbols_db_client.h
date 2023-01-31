#ifndef STONKS_APP_SERVICE_INJECTOR_SERVICE_INJ_SYMBOLS_DB_CLIENT_H_
#define STONKS_APP_SERVICE_INJECTOR_SERVICE_INJ_SYMBOLS_DB_CLIENT_H_

#include "core_i_symbols_db.h"
#include "cpp_meta_thread_safe.h"
#include "cpp_mutex.h"
#include "di_bind_type_to_factory_function.h"
#include "di_make_injector.h"
#include "log_i_logger.h"
#include "networkx_client.h"
#include "restsdk_rest_request_sender.h"
#include "service_client_options.h"
#include "service_inj_client_server.h"
#include "service_symbols_db.h"

namespace stonks::service::inj {
inline auto CreateSymbolsDbClientInjector(
    const ClientOptions<core::ISymbolsDb> &options) {
  return di::MakeInjector(
      CreateClientInjector<SymbolsDb>(options),
      di::BindTypeToFactoryFunction<
          cpp::meta::ThreadSafe<cpp::NnUp<core::ISymbolsDb>>,
          +[](networkx::Uri<core::ISymbolsDb> uri,
              cpp::NnUp<log::ILogger> logger) {
            auto mutex = cpp::MutexVariant{cpp::Mutex{}};
            auto request_sender = cpp::MakeNnUp<restsdk::RestRequestSender>(
                std::move(logger), std::move(mutex));
            auto client = networkx::Client<core::ISymbolsDb>{
                std::move(uri), std::move(request_sender)};
            auto symbols_db = cpp::MakeNnUp<SymbolsDb>(std::move(client));
            return cpp::meta::AssumeThreadSafe<cpp::NnUp<core::ISymbolsDb>>(
                std::move(symbols_db));
          }>());
}
}  // namespace stonks::service::inj

#endif  // STONKS_APP_SERVICE_INJECTOR_SERVICE_INJ_SYMBOLS_DB_CLIENT_H_
