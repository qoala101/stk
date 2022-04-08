#include <pplx/pplxtasks.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <variant>
#include <vector>

#include "breakout_strategy_service.h"
#include "finance_enums.h"
#include "get_candlestick_chart_data_service.h"
#include "get_file_service.h"
#include "get_symbols_service.h"
#include "order_proxy_service.h"
#include "utils.h"

int main(int, const char *[]) {
  using ServiceVariant =
      std::variant<stonks::GetFileService, stonks::GetSymbolsService,
                   stonks::GetCandlestickChartDataService,
                   stonks::BreakoutStrategyService, stonks::OrderProxyService>;

  auto services = std::vector<std::unique_ptr<ServiceVariant>>{};
  services.emplace_back(std::make_unique<ServiceVariant>());
  services.back()->emplace<stonks::GetFileService>();
  services.emplace_back(std::make_unique<ServiceVariant>());
  services.back()->emplace<stonks::GetSymbolsService>();
  services.emplace_back(std::make_unique<ServiceVariant>());
  services.back()->emplace<stonks::GetCandlestickChartDataService>();
  services.emplace_back(std::make_unique<ServiceVariant>());
  services.back()->emplace<stonks::OrderProxyService>();
  services.emplace_back(std::make_unique<ServiceVariant>());
  services.back()->emplace<stonks::BreakoutStrategyService>();

  for (auto &service : services) {
    std::visit([](auto &service) { service.Start(); }, *service);
  }

  spdlog::info("Started {} services", services.size());
  getchar();
  spdlog::info("Stopping...");

  auto stop_tasks = std::vector<pplx::task<void>>{};

  for (auto &service : services) {
    auto stop_task =
        std::visit([](auto &service) { return service.Stop(); }, *service);
    stop_tasks.emplace_back(std::move(stop_task));
  }

  for (auto &stop_task : stop_tasks) {
    stop_task.wait();
  }

  spdlog::info("Stopped");

  return 0;
}