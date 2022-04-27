#include "prices_stream.h"

#include <gtest/gtest.h>

#include <range/v3/action/push_back.hpp>

#include "finance_enums.h"
#include "finance_types.h"
#include "utils.h"

namespace {
const auto kDefaultSymbol =
    stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"};
}

TEST(PricesStream, Stream) {
  auto stream = stonks::finance::PricesStream{kDefaultSymbol};
  stream.Start();

  std::this_thread::sleep_for(std::chrono::seconds{5});

  auto prices = stream.GetPrices();
  EXPECT_FALSE(prices.empty());

  auto old_size = prices.size();
  prices |= ranges::actions::push_back(stream.GetPrices(prices.size()));
  EXPECT_GT(prices.size(), old_size);

  old_size = prices.size();
  prices |= ranges::actions::push_back(stream.GetPrices(prices.size()));
  EXPECT_GT(prices.size(), old_size);

  old_size = prices.size();
  prices |= ranges::actions::push_back(stream.GetPrices(prices.size()));
  EXPECT_GT(prices.size(), old_size);
}