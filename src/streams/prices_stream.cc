#include "prices_stream.h"

#include <spdlog/spdlog.h>

#include <gsl/util>
#include <range/v3/to_container.hpp>
#include <range/v3/view/drop.hpp>
#include <thread>

#include "finance_api.h"

namespace stonks::finance {
PricesStream::PricesStream(Symbol symbol, std::chrono::milliseconds interval)
    : symbol_{std::move(symbol)}, interval_{interval}, keep_alive_{true} {}

PricesStream::~PricesStream() {
  Stop();
  thread_.join();
}

void PricesStream::Start() {
  if (const auto already_started = thread_.joinable()) {
    return;
  }

  thread_ = std::thread{[&symbol = symbol_, interval = interval_,
                         &keep_alive = keep_alive_, &prices = prices_,
                         &prices_mutex = prices_mutex_,
                         &prices_cond_var = prices_cond_var_]() {
    while (keep_alive.load(std::memory_order_relaxed)) {
      const auto sleep_finally =
          gsl::finally([interval]() { std::this_thread::sleep_for(interval); });

      const auto price = GetSymbolPrice(symbol);

      if (!price.has_value()) {
        spdlog::error("Cannot get price");
        continue;
      }

      {
        auto lock = std::lock_guard{prices_mutex};

        prices.emplace_back(
            TimeDouble{.time = utils::GetUnixTime(), .value = *price});
        prices_cond_var.notify_all();
      }
    }
  }};
}

void PricesStream::Stop() {
  keep_alive_.store(false, std::memory_order::relaxed);
}

std::vector<TimeDouble> PricesStream::GetNextPrices() {
  const auto check_if_has_prices = [&prices = prices_]() {
    return !prices.empty();
  };

  auto prices = std::vector<TimeDouble>{};

  {
    auto lock = std::unique_lock{prices_mutex_};
    prices_cond_var_.wait(lock, check_if_has_prices);

    prices.swap(prices_);
  }

  return prices;
}
}  // namespace stonks::finance