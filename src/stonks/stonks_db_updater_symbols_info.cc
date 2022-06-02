#include "stonks_db_updater_symbols_info.h"

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <gsl/assert>
#include <range/v3/all.hpp>

#include "client_binance.h"
#include "finance_types.h"

namespace stonks {
namespace {
auto Logger() -> spdlog::logger & {
  static auto logger = []() {
    auto logger = spdlog::stdout_color_mt("DbUpdaterSymbolsInfo");
    logger->set_level(spdlog::level::debug);
    return logger;
  }();

  return *logger;
}
}  // namespace

class DbUpdaterSymbolsInfo::Impl {
 public:
  explicit Impl(std::shared_ptr<StonksDb> stonks_db)
      : stonks_db_{std::move(stonks_db)} {
    Expects(stonks_db_ != nullptr);

    const auto binance_client = client::Binance{};
    const auto exchange_info = binance_client.GetExchangeInfo();

    const auto get_base_asset = [](const auto &symbol_info) {
      return symbol_info.base_asset;
    };
    const auto get_quote_asset = [](const auto &symbol_info) {
      return symbol_info.quote_asset;
    };
    auto assets =
        ranges::views::concat(
            exchange_info.symbols | ranges::views::transform(get_base_asset),
            exchange_info.symbols | ranges::views::transform(get_quote_asset)) |
        ranges::to_vector | ranges::actions::sort | ranges::actions::unique;
    stonks_db_->UpdateAssets(std::move(assets));

    const auto to_symbol_info = [](const auto &symbol_info) {
      return finance::SymbolInfo{.symbol = symbol_info.symbol,
                                 .base_asset = symbol_info.base_asset,
                                 .quote_asset = symbol_info.quote_asset,
                                 .min_base_amount = symbol_info.min_quantity,
                                 .min_quote_amount = symbol_info.min_notional,
                                 .base_step = symbol_info.step_size,
                                 .quote_step = symbol_info.tick_size};
    };
    auto symbols_info = exchange_info.symbols |
                        ranges::views::transform(to_symbol_info) |
                        ranges::to_vector;
    stonks_db_->UpdateSymbolsInfo(std::move(symbols_info));
  }

  Impl(const Impl &) = delete;
  Impl(Impl &&) noexcept = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) noexcept -> Impl & = default;

  ~Impl() = default;

 private:
  std::shared_ptr<StonksDb> stonks_db_{};
};

DbUpdaterSymbolsInfo::DbUpdaterSymbolsInfo(std::shared_ptr<StonksDb> stonks_db)
    : impl_{std::make_unique<Impl>(std::move(stonks_db))} {}

DbUpdaterSymbolsInfo::DbUpdaterSymbolsInfo(DbUpdaterSymbolsInfo &&) noexcept =
    default;

auto DbUpdaterSymbolsInfo::operator=(DbUpdaterSymbolsInfo &&) noexcept
    -> DbUpdaterSymbolsInfo & = default;

DbUpdaterSymbolsInfo::~DbUpdaterSymbolsInfo() = default;
}  // namespace stonks