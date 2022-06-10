#include "aws_api.h"

#include <aws/core/Aws.h>
#include <aws/core/monitoring/MonitoringManager.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace stonks::aws {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger& {
  static auto logger = spdlog::stdout_color_mt("AwsApi");
  return *logger;
}

auto Options() -> const Aws::SDKOptions& {
  static const auto kOptions = Aws::SDKOptions{};
  return kOptions;
}
}  // namespace

auto AwsApi::Instance() -> std::shared_ptr<AwsApi> {
  class PublicAwsApi : public AwsApi {};
  static auto instance = std::make_shared<PublicAwsApi>();
  return instance;
}

AwsApi::~AwsApi() {
  Aws::ShutdownAPI(Options());
  Logger().info("AWS API shut down");
}

AwsApi::AwsApi() {
  Aws::InitAPI(Options());
  Logger().info("AWS API initialized");
}
}  // namespace stonks::aws