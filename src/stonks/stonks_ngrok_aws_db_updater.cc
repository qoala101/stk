#include "stonks_ngrok_aws_db_updater.h"

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <boost/process/child.hpp>
#include <boost/process/io.hpp>

#include "aws_dynamo_db.h"
#include "ngrok_client.h"
#include "ngrok_types.h"
#include "proxy.h"

namespace stonks {
namespace {
auto Logger() -> spdlog::logger & {
  static auto logger = []() {
    auto logger = spdlog::stdout_color_mt("NgrokAwsDbUpdater");
    logger->set_level(spdlog::level::debug);
    return logger;
  }();

  return *logger;
}

constexpr auto kReattemptDuration = std::chrono::seconds{10};
constexpr auto kUpdateInterval = std::chrono::minutes{1};
}  // namespace

class NgrokAwsDbUpdater::Impl {
 public:
  explicit Impl() : thread_{std::bind_front(&Impl::Execute, this)} {}

  Impl(const Impl &) = delete;
  Impl(Impl &&) noexcept = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) noexcept -> Impl & = default;

  ~Impl() {
    ngrok_process_.terminate();
    ngrok_process_.join();
  }

  static auto CheckIfStopRequested(const std::stop_token &stop_token) -> bool {
    return stop_token.stop_requested();
  }

  static auto CheckIfStopRequestedFor(const std::stop_token &stop_token,
                                      std::chrono::nanoseconds duration)
      -> bool {
    static constexpr auto kTimeStep = std::chrono::seconds{1};
    const auto kNumIntervals = duration / kTimeStep;

    for (auto i = 0; i < kNumIntervals; ++i) {
      if (CheckIfStopRequested(stop_token)) {
        return true;
      }

      std::this_thread::sleep_for(kTimeStep);
    }

    return false;
  }

  void Execute(const std::stop_token &stop_token) {
    const auto table = aws::DynamoDb::KeyValueTable{"stonks"};
    dynamo_db_.CreateTableIfNotExists(table);

    while (true) {
      if (const auto not_running = !ngrok_process_.running()) {
        ngrok_process_ = boost::process::child{
            "ngrok http " + std::to_string(network::Proxy::kPort),
            boost::process::std_out.null()};
        ngrok_process_.wait_for(kReattemptDuration);

        if (!ngrok_process_.running()) {
          Logger().error("Cannot start ngrok, waiting to reattempt");

          if (CheckIfStopRequestedFor(stop_token, kReattemptDuration)) {
            return;
          }

          continue;
        }
      }

      if (CheckIfStopRequested(stop_token)) {
        return;
      }

      auto tunnel = ngrok::Tunnel{};

      try {
        tunnel = ngrok_client_.GetTunnel();
      } catch (const std::exception &exception) {
        Logger().error("Cannot get tunnel from ngrok {}, waiting to reattempt",
                       exception.what());

        if (CheckIfStopRequestedFor(stop_token, kReattemptDuration)) {
          return;
        }

        continue;
      }

      if (CheckIfStopRequested(stop_token)) {
        return;
      }

      if (const auto public_url_changed =
              tunnel.public_url != last_public_url_) {
        try {
          dynamo_db_.AddOrUpdateItem(table, {"public_url", tunnel.public_url});
        } catch (const std::exception &exception) {
          Logger().error(
              "Cannot add item to Dynamo DB {}, waiting to reattempt",
              exception.what());

          if (CheckIfStopRequestedFor(stop_token, kReattemptDuration)) {
            return;
          }

          continue;
        }

        last_public_url_ = tunnel.public_url;
        Logger().debug("Updated public URL in Dynamo DB {}", last_public_url_);
      }

      if (CheckIfStopRequestedFor(stop_token, kUpdateInterval)) {
        return;
      }
    }
  }

 private:
  boost::process::child ngrok_process_{};
  ngrok::NgrokClient ngrok_client_{};
  aws::DynamoDb dynamo_db_{};
  std::string last_public_url_{};
  std::jthread thread_{};
};

NgrokAwsDbUpdater::NgrokAwsDbUpdater() : impl_{std::make_unique<Impl>()} {}

NgrokAwsDbUpdater::NgrokAwsDbUpdater(NgrokAwsDbUpdater &&) noexcept = default;

auto NgrokAwsDbUpdater::operator=(NgrokAwsDbUpdater &&) noexcept
    -> NgrokAwsDbUpdater & = default;

NgrokAwsDbUpdater::~NgrokAwsDbUpdater() = default;
}  // namespace stonks