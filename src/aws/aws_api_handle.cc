#include "aws_api_handle.h"

#include <aws/core/Aws.h>
#include <aws/core/monitoring/MonitoringManager.h>
#include <bits/exception.h>

#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"

namespace stonks::aws {
namespace {
[[nodiscard]] auto Options() -> auto & {
  static const auto kOptions = Aws::SDKOptions{};
  return kOptions;
}
}  // namespace

namespace detail {
class ApiHandleImpl {
 public:
  explicit ApiHandleImpl(cpp::NnUp<log::ILogger> logger)
      : logger_{std::move(logger)} {
    Aws::InitAPI(Options());
  }

  ApiHandleImpl(const ApiHandleImpl &) = delete;
  ApiHandleImpl(ApiHandleImpl &&) noexcept = delete;

  auto operator=(const ApiHandleImpl &) -> ApiHandleImpl & = delete;
  auto operator=(ApiHandleImpl &&) noexcept -> ApiHandleImpl & = delete;

  ~ApiHandleImpl() {
    try {
      Aws::ShutdownAPI(Options());
    } catch (const std::exception &e) {
      logger_->LogErrorCondition(e.what());
    }
  }

 private:
  cpp::NnUp<log::ILogger> logger_;
};
}  // namespace detail

namespace {
[[nodiscard]] auto GetCachedApiHandle(cpp::NnUp<log::ILogger> logger) {
  static auto last_instance = cpp::Wp<detail::ApiHandleImpl>{};

  auto last_instance_lock = last_instance.lock();

  if (const auto last_instance_is_alive = last_instance_lock != nullptr) {
    Ensures(!last_instance.expired());
    return cpp::AssumeNn(last_instance_lock);
  }

  auto new_instance = cpp::MakeNnSp<detail::ApiHandleImpl>(std::move(logger));

  last_instance = new_instance.as_nullable();
  Ensures(!last_instance.expired());

  return new_instance;
}
}  // namespace

ApiHandle::ApiHandle(cpp::NnUp<log::ILogger> logger)
    : impl_{GetCachedApiHandle(std::move(logger))} {}
}  // namespace stonks::aws