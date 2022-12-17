#ifndef STONKS_NGROK_NGROK_API_TRAITS_H_
#define STONKS_NGROK_NGROK_API_TRAITS_H_

#include "networkx_common.h"
#include "networkx_types.h"
#include "ngrok_api.h"

namespace stonks::networkx {
template <>
struct EndpointFunctionTraits<&ngrok::NgrokApi::tunnels> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct ClientServerTypeTraits<ngrok::NgrokApi> {
  static constexpr auto kEndpointFunctions =
      EndpointFunctionList(&ngrok::NgrokApi::tunnels);
};
}  // namespace stonks::networkx

#endif  // STONKS_NGROK_NGROK_API_TRAITS_H_
