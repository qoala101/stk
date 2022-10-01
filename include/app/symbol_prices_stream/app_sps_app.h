#ifndef STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_APP_H_
#define STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_APP_H_

#include "core_types.h"
#include "cpp_not_null.h"
#include "network_i_ws_client.h"
#include "network_ws_connection.h"

namespace stonks::app::sps {
class App {
 public:
  App(cpp::NnUp<network::IWsClient> ws_client, core::Symbol symbol);

 private:
  network::WsConnection ws_connection_;
};
}  // namespace stonks::app::sps

#endif  // STONKS_APP_SYMBOL_PRICES_STREAM_APP_SPS_APP_H_
