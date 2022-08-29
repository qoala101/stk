#include "aws_api_handle.h"

#include <aws/core/Aws.h>
#include <aws/core/monitoring/MonitoringManager.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>

namespace stonks::aws {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger& {
  static auto logger = spdlog::stdout_color_mt("ApiHandle");
  return *logger;
}

[[nodiscard]] auto Options() -> const Aws::SDKOptions& {
  static const auto kOptions = Aws::SDKOptions{};
  return kOptions;
}
}  // namespace

auto ApiHandle::Instance() -> cpp::NnSp<ApiHandle> {
  class EnableMakeSp : public ApiHandle {};
  static auto instance = cpp::MakeNnSp<EnableMakeSp>();
  return instance;
}

ApiHandle::~ApiHandle() {
  Aws::ShutdownAPI(Options());
  Logger().info("AWS API shut down");
}

ApiHandle::ApiHandle() {
  Aws::InitAPI(Options());
  Logger().info("AWS API initialized");
}
}  // namespace stonks::aws