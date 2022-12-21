#include "core_symbol_prices_streams_controller.h"

#include <fmt/format.h>

#include <cppcoro/task.hpp>

#include "core_types.h"

namespace stonks::core {
SymbolPricesStreamsController::SymbolPricesStreamsController(
    cpp::NnUp<log::ILogger> logger)
    : logger_{std::move(logger)} {}

auto SymbolPricesStreamsController::GetSymbolForStream() const
    -> cppcoro::task<Symbol> {
  const auto symbol = Symbol{"BTCUSDT"};
  logger_->LogImportantEvent(fmt::format("Given {} for stream", *symbol));
  co_return symbol;
}
}  // namespace stonks::core