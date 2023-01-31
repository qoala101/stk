#include "service_app_uri_exposer.h"

#include <cppcoro/sync_wait.hpp>
#include <type_traits>
#include <utility>

#include "cpp_timer_builder.h"

namespace vh::stk::service {
AppUriExposer::AppUriExposer(ConstructorArgs args)
    : expose_uri_timer_{
          cpp::Execute([exposer = std::move(args.exposer)]() mutable {
            cppcoro::sync_wait(exposer.ExposeNgrokUriIfChanged());
          })
              .Every(args.expose_uri_interval)
              .IfThrowsReattemptEvery(args.reattempt_interval)
              .Start()} {}
}  // namespace vh::stk::service