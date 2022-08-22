#include "price_ticks_db_stream.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "finance_db_service.h"

namespace {
TEST(PriceTicksDbStream, Buffer) {
  auto finance_db_service = stonks::FinanceDbService{};
  finance_db_service.Start().wait();

  const auto kBufferSize = 100;
  auto stream = stonks::finance::PriceTicksDbStream{std::nullopt, kBufferSize};

  for (auto i = 0; i < kBufferSize / 2; ++i) {
    const auto ticks = stream.GetNextPriceTicks();
    ASSERT_TRUE(ticks.has_value());
    EXPECT_EQ(ticks->size(), 1);
  }

  finance_db_service.Stop().wait();

  for (auto i = kBufferSize / 2; i < kBufferSize; ++i) {
    const auto ticks = stream.GetNextPriceTicks();
    ASSERT_TRUE(ticks.has_value());
    EXPECT_EQ(ticks->size(), 1);
  }

  const auto ticks = stream.GetNextPriceTicks();
  ASSERT_FALSE(ticks.has_value());
}

TEST(PriceTicksDbStream, LastTime) {
  auto finance_db_service = stonks::FinanceDbService{};
  finance_db_service.Start().wait();

  const auto kBufferSize = 100;
  auto stream = stonks::finance::PriceTicksDbStream{std::nullopt, kBufferSize};

  auto last_time = std::chrono::milliseconds{0};

  for (auto i = 0; i < kBufferSize; ++i) {
    const auto ticks = stream.GetNextPriceTicks();
    ASSERT_TRUE(ticks.has_value());
    EXPECT_EQ(ticks->size(), 1);

    if (i == kBufferSize - 1) {
      last_time = (*ticks)[0].time;
    }
  }

  const auto ticks = stream.GetNextPriceTicks();
  ASSERT_TRUE(ticks.has_value());
  EXPECT_EQ(ticks->size(), 1);
  EXPECT_GT(ticks->back().time, last_time);

  finance_db_service.Stop().wait();
}
}  // namespace