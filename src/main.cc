#include <pplx/pplxtasks.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <variant>
#include <vector>

#include "breakout_strategy_service.h"
#include "finance_enums.h"
#include "finance_types.h"
#include "get_candle_chart_data_service.h"
#include "get_file_service.h"
#include "get_symbols_service.h"
#include "mean_average_strategy_service.h"
#include "order_monitor_service.h"
#include "order_proxy_service.h"
#include "utils.h"

int main(int, const char *[]) {
  using ServiceVariant =
      std::variant<stonks::GetFileService, stonks::GetSymbolsService,
                   stonks::GetCandleChartDataService,
                   stonks::BreakoutStrategyService, stonks::OrderProxyService,
                   stonks::OrderMonitorService,
                   stonks::MeanAverageStrategyService>;

  auto services = std::vector<std::unique_ptr<ServiceVariant>>{};

  services.emplace_back(std::make_unique<ServiceVariant>())
      ->emplace<stonks::GetFileService>();

  // services.emplace_back(std::make_unique<ServiceVariant>())
  //     ->emplace<stonks::GetSymbolsService>();

  // services.emplace_back(std::make_unique<ServiceVariant>())
  //     ->emplace<stonks::GetCandleChartDataService>();

  services.emplace_back(std::make_unique<ServiceVariant>())
      ->emplace<stonks::OrderMonitorService>();

  services.emplace_back(std::make_unique<ServiceVariant>())
      ->emplace<stonks::OrderProxyService>();

  // const auto symbols = std::vector<stonks::finance::Symbol>{
  //     // stonks::finance::Symbol{.base_asset = "BTC", .quote_asset = "USDT"},
  //     stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"},
  //     // stonks::finance::Symbol{.base_asset = "BNB", .quote_asset = "USDT"},
  //     // stonks::finance::Symbol{.base_asset = "NEO", .quote_asset = "USDT"},
  //     // stonks::finance::Symbol{.base_asset = "LTC", .quote_asset = "USDT"},
  //     // stonks::finance::Symbol{.base_asset = "QTUM", .quote_asset =
  //     "USDT"},
  //     // stonks::finance::Symbol{.base_asset = "ADA", .quote_asset = "USDT"},
  //     // stonks::finance::Symbol{.base_asset = "XRP", .quote_asset = "USDT"},
  //     // stonks::finance::Symbol{.base_asset = "EOS", .quote_asset = "USDT"},
  //     // stonks::finance::Symbol{.base_asset = "TUSD", .quote_asset =
  //     "USDT"},
  //     // stonks::finance::Symbol{.base_asset = "IOTA", .quote_asset =
  //     "USDT"},
  //     // stonks::finance::Symbol{.base_asset = "XLM", .quote_asset = "USDT"},
  //     // stonks::finance::Symbol{.base_asset = "ONT", .quote_asset = "USDT"},
  //     // stonks::finance::Symbol{.base_asset = "TRX", .quote_asset = "USDT"},
  //     // stonks::finance::Symbol{.base_asset = "ETC", .quote_asset = "USDT"}
  // };

  // for (const auto &symbol : symbols) {
  //   // services.emplace_back(std::make_unique<ServiceVariant>())
  //   //     ->emplace<stonks::BreakoutStrategyService>(symbol);

  //   services.emplace_back(std::make_unique<ServiceVariant>())
  //       ->emplace<stonks::MeanAverageStrategyService>(symbol);
  // }

  auto symbol =
      stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"};
  auto symbol_precision = 0.0001;
  auto comission = 0.001;
  auto profit = 0.001;

  // symbol = stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"};
  // symbol_precision = 0.0001;
  // comission = 0.001;
  // profit = 0.001;
  // services.emplace_back(std::make_unique<ServiceVariant>())
  //     ->emplace<stonks::MeanAverageStrategyService>(symbol, symbol_precision,
  //                                                   comission, profit);

  symbol = stonks::finance::Symbol{.base_asset = "BTC", .quote_asset = "USDT"};
  symbol_precision = 0.00001;
  comission = 0.001;
  profit = 0.002;
  services.emplace_back(std::make_unique<ServiceVariant>())
      ->emplace<stonks::MeanAverageStrategyService>(symbol, symbol_precision,
                                                    comission, profit);

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