#ifndef STONKS_BINANCE_BINANCE_API_H_
#define STONKS_BINANCE_BINANCE_API_H_

#include <cppcoro/task.hpp>

#include "binance_types.h"
#include "di_factory.h"
#include "network_i_rest_request_sender.h"

namespace stonks::binance {
/**
 * @brief Client to Binance REST API.
 */
class BinanceApi {
 public:
  explicit BinanceApi(
      di::Factory<network::IRestRequestSender> request_sender_factory);

  BinanceApi(const BinanceApi &) = delete;
  BinanceApi(BinanceApi &&) noexcept;

  auto operator=(const BinanceApi &) -> BinanceApi & = delete;
  auto operator=(BinanceApi &&) noexcept -> BinanceApi &;

  ~BinanceApi();

  auto exchangeInfo [[nodiscard]] () const -> cppcoro::task<ExchangeInfo>;

 private:
  class Impl;

  cpp::NnUp<Impl> impl_;
};
}  // namespace stonks::binance

#endif  // STONKS_BINANCE_BINANCE_API_H_
