#ifndef STONKS_NETWORKX_NETWORKX_MAKE_SERVER_FOR_H_
#define STONKS_NETWORKX_NETWORKX_MAKE_SERVER_FOR_H_

#include <tuple>
#include <type_traits>
#include <utility>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_for_each_index.h"
#include "cpp_not_null.h"
#include "network_auto_parsable_request.h"
#include "network_rest_server_builder.h"
#include "networkx_client_server_type_traits_facade.h"
#include "networkx_concepts.h"  // IWYU pragma: keep
#include "networkx_endpoint_function_traits_facade.h"
#include "networkx_types.h"

namespace stonks::networkx {
namespace detail {
template <EndpointParam Param>
auto GetParamValue [[nodiscard]] (network::AutoParsableRestRequest &request,
                                  const Param &param) {
  if constexpr (std::is_same_v<Param, RequestBody>) {
    return request.Body();
  } else {
    return request.Param(param);
  }
}

template <cpp::MemberFunction auto kFunction, ClientServerType Target>
  requires EndpointFunction<kFunction> &&
           cpp::MemberFunctionOf<decltype(kFunction), Target>
auto InvokeWithRequestParams
    [[nodiscard]] (Target &target, network::AutoParsableRestRequest &request) {
  using FunctionTraits = EndpointFunctionTraitsFacade<kFunction>;

  if constexpr (FunctionTraits::HasParams()) {
    return std::apply(
        [&target, &request]<typename... Params>(Params &&...params) {
          return (target.*kFunction)(
              GetParamValue(request, std::forward<Params>(params))...);
        },
        FunctionTraits::kParams);
  } else {
    return (target.*kFunction)();
  }
}

template <ClientServerType Target>
void SetEndpointHandlers(network::RestServerBuilder &server_builder,
                         const cpp::NnSp<Target> &target) {
  using TargetTraits = ClientServerTypeTraitsFacade<Target>;

  cpp::ForEachIndex<TargetTraits::GetNumEndpoints()>(
      [&server_builder, &target]<typename Current>(Current) {
        constexpr auto function =
            TargetTraits::template GetEndpointFunction<Current::kIndex>();

        server_builder.Handling(
            EndpointFunctionTraitsFacade<function>::AsTypedEndpoint(),
            [target](auto request) {
              return InvokeWithRequestParams<function>(*target, request);
            });
      });
}
}  // namespace detail

/**
 * @brief Creates REST server for the target object on specified URI.
 * Server would handle request according to the object client-server traits.
 */
template <ClientServerType Target>
auto MakeServerFor [[nodiscard]] (const cpp::NnSp<Target> &target,
                                  network::RestServerBuilder &server_builder) {
  detail::SetEndpointHandlers(server_builder, target);
  return server_builder.Start();
}
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_MAKE_SERVER_FOR_H_
