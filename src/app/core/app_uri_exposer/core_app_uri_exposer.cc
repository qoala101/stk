#include "core_app_uri_exposer.h"

#include <cppcoro/sync_wait.hpp>
#include <type_traits>
#include <utility>

#include "cpp_timer_builder.h"

namespace stonks::core {
AppUriExposer::AppUriExposer(absl::Duration expose_uri_interval,
                             aue::Impl exposer,
                             absl::Duration reattempt_interval)
    : expose_uri_timer_{cpp::Execute([exposer = std::move(exposer)]() {
                          cppcoro::sync_wait(exposer.ExposeNgrokUri());
                        })
                            .Every(expose_uri_interval)
                            .IfThrowsReattemptEvery(reattempt_interval)
                            .Start()} {}
}  // namespace stonks::core