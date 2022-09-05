#include "aws_api_handle.h"

#include <aws/core/Aws.h>
#include <aws/core/monitoring/MonitoringManager.h>
#include <bits/exception.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <gsl/assert>
#include <memory>

#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"
#include "not_null.hpp"

namespace stonks::aws {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger& {
  static auto logger = spdlog::stdout_color_mt("aws::ApiHandle");
  return *logger;
}

[[nodiscard]] auto Options() -> const Aws::SDKOptions& {
  static const auto kOptions = Aws::SDKOptions{};
  return kOptions;
}
}  // namespace

auto ApiHandle::Instance() -> cpp::NnSp<ApiHandle> {
  static auto last_instance = cpp::Wp<ApiHandle>{};

  auto last_instance_lock = last_instance.lock();

  if (const auto last_instance_is_alive = last_instance_lock != nullptr) {
    Ensures(!last_instance.expired());
    return cpp::AssumeNn(last_instance_lock);
  }

  class EnableMakeSp : public ApiHandle {};
  auto new_instance = cpp::MakeNnSp<EnableMakeSp>();

  last_instance = new_instance.as_nullable();
  Ensures(!last_instance.expired());

  return new_instance;
}

ApiHandle::~ApiHandle() try {
  Aws::ShutdownAPI(Options());
} catch (const std::exception& e) {
  Logger().error(e.what());
}

ApiHandle::ApiHandle() { Aws::InitAPI(Options()); }
}  // namespace stonks::aws