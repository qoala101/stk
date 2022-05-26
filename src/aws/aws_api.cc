#include "aws_api.h"

#include <aws/core/Aws.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace stonks::aws {
namespace {
auto Logger() -> spdlog::logger& {
  static auto logger = []() {
    auto logger = spdlog::stdout_color_mt("AwsApi");
    logger->set_level(spdlog::level::debug);
    return logger;
  }();

  return *logger;
}
}  // namespace

namespace {
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
  Logger().debug("AWS API shut down");
}

AwsApi::AwsApi() {
  Aws::InitAPI(Options());
  Logger().debug("AWS API initialized");
}
}  // namespace stonks::aws