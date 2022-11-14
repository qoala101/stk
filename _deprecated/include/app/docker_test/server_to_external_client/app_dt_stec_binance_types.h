#ifndef STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_BINANCE_TYPES_H_
#define STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_BINANCE_TYPES_H_

namespace stonks::app::dt::stec::binance {
struct AvgPrice {
  int mins{};
  double price{};
};
}  // namespace stonks::app::dt::stec::binance

#endif  // STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_BINANCE_TYPES_H_
