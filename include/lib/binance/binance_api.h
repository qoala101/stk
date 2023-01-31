#ifndef STONKS_BINANCE_BINANCE_API_H_
#define STONKS_BINANCE_BINANCE_API_H_

#include <cppcoro/task.hpp>

#include "binance_types.h"
#include "cpp_not_null.h"
#include "network_i_rest_request_sender.h"

namespace vh::binance {
/**
 * @brief Client to Binance REST API.
 */
class BinanceApi {
 public:
  explicit BinanceApi(cpp::NnUp<network::IRestRequestSender> request_sender);

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
}  // namespace vh::binance

#endif  // STONKS_BINANCE_BINANCE_API_H_
