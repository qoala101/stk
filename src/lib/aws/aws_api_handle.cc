/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "aws_api_handle.h"

#include <aws/core/Aws.h>
#include <aws/core/monitoring/MonitoringManager.h>
#include <fmt/core.h>

#include <exception>
#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"

namespace vh::aws {
namespace detail {
class ApiHandleImpl {
 public:
  explicit ApiHandleImpl(cpp::NnUp<log::ILogger> logger)
      : logger_{std::move(logger)} {
    Aws::InitAPI(Aws::SDKOptions{});
  }

  ApiHandleImpl(const ApiHandleImpl &) = delete;
  ApiHandleImpl(ApiHandleImpl &&) noexcept = delete;

  auto operator=(const ApiHandleImpl &) -> ApiHandleImpl & = delete;
  auto operator=(ApiHandleImpl &&) noexcept -> ApiHandleImpl & = delete;

  ~ApiHandleImpl() {
    try {
      Aws::ShutdownAPI(Aws::SDKOptions{});
    } catch (const std::exception &e) {
      logger_->LogErrorCondition(
          fmt::format("Couldn't close AWS handle: {}", e.what()));
    }
  }

 private:
  cpp::NnUp<log::ILogger> logger_;
};
}  // namespace detail

namespace {
auto GetCachedApiHandle [[nodiscard]] (cpp::NnUp<log::ILogger> logger) {
  static auto last_instance = cpp::Wp<detail::ApiHandleImpl>{};

  if (auto last_instance_lock = last_instance.lock()) {
    Ensures(!last_instance.expired());
    return cpp::AssumeNn(std::move(last_instance_lock));
  }

  auto new_instance = cpp::MakeNnSp<detail::ApiHandleImpl>(std::move(logger));

  last_instance = new_instance.as_nullable();
  Ensures(!last_instance.expired());

  return new_instance;
}
}  // namespace

ApiHandle::ApiHandle(cpp::NnUp<log::ILogger> logger)
    : impl_{GetCachedApiHandle(std::move(logger))} {}
}  // namespace vh::aws