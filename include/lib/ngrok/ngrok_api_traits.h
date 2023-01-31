#ifndef VH_NGROK_API_TRAITS_H_
#define VH_NGROK_API_TRAITS_H_

#include "networkx_common.h"
#include "networkx_types.h"
#include "ngrok_api.h"

namespace vh::networkx {
template <>
struct EndpointFunctionTraits<&ngrok::NgrokApi::tunnels> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct ClientServerTypeTraits<ngrok::NgrokApi> {
  static constexpr auto kEndpointFunctions =
      EndpointFunctionList(&ngrok::NgrokApi::tunnels);
};
}  // namespace vh::networkx

#endif  // VH_NGROK_API_TRAITS_H_
