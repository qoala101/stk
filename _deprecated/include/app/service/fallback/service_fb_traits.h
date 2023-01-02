#ifndef STONKS_APP_SERVICE_FALLBACK_SERVICE_FB_TRAITS_H_
#define STONKS_APP_SERVICE_FALLBACK_SERVICE_FB_TRAITS_H_

#include "networkx_common.h"
#include "networkx_types.h"
#include "service_fallback.h"

namespace stonks::networkx {
template <>
struct EndpointFunctionTraits<&service::Fallback::HandleNotSupportedUri> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct ClientServerTypeTraits<service::Fallback> {
  static constexpr auto kName = "fallback";
  static constexpr auto kDefaultPort = "30002";
  static constexpr auto kEndpointFunctions =
      EndpointFunctionList(&service::Fallback::HandleNotSupportedUri);
};
}  // namespace stonks::networkx

#endif  // STONKS_APP_SERVICE_FALLBACK_SERVICE_FB_TRAITS_H_
