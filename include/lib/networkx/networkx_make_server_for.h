#ifndef VH_NETWORKX_MAKE_SERVER_FOR_H_
#define VH_NETWORKX_MAKE_SERVER_FOR_H_

#include <cppcoro/task.hpp>
#include <tuple>
#include <type_traits>
#include <utility>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_for_each_index.h"
#include "cpp_not_null.h"
#include "cpp_share.h"
#include "network_auto_parsable_request.h"
#include "network_rest_server_builder.h"
#include "networkx_client_server_type_traits_facade.h"
#include "networkx_common.h"
#include "networkx_concepts.h"  // IWYU pragma: keep
#include "networkx_endpoint_function_traits_facade.h"
#include "networkx_types.h"
#include "networkx_uri.h"

namespace vh::networkx {
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

template <cpp::MemberFunction auto kFunction, ClientServerType Target,
          typename FunctionTraits = EndpointFunctionTraitsFacade<kFunction>,
          typename ResultType = typename FunctionTraits::ResultType>
  requires EndpointFunction<kFunction> &&
           cpp::MemberFunctionOf<decltype(kFunction), Target>
           auto CallWithRequestParams
           [[nodiscard]] (Target &target,
                          network::AutoParsableRestRequest &request)
           -> cppcoro::task<ResultType> {
  co_return co_await std::apply(
      [&target, &request]<typename... Params>(
          Params &&...params) -> cppcoro::task<ResultType> {
        co_return co_await (target.*kFunction)(
            GetParamValue(request, std::forward<Params>(params))...);
      },
      FunctionTraits::kParams);
}

template <ClientServerType Target>
void SetEndpointHandlers(network::RestServerBuilder &server_builder,
                         const cpp::NnSp<Target> &target) {
  using TargetTraits = ClientServerTypeTraitsFacade<Target>;

  cpp::ForEachIndex<TargetTraits::GetNumEndpoints()>(
      [&server_builder, &target]<typename Current>(Current) {
        constexpr auto function =
            TargetTraits::template GetEndpointFunction<Current::kIndex>();

        using FunctionTraits = EndpointFunctionTraitsFacade<function>;

        auto endpoint = FunctionTraits::AsTypedEndpoint();

        using ResultType = typename FunctionTraits::ResultType;

        if constexpr (FunctionTraits::HasParams()) {
          server_builder.Handling(
              std::move(endpoint),
              [target](auto request) -> cppcoro::task<ResultType> {
                co_return co_await CallWithRequestParams<function>(*target,
                                                                   request);
              });
        } else {
          server_builder.Handling(std::move(endpoint),
                                  [target]() -> cppcoro::task<ResultType> {
                                    co_return co_await (*target.*function)();
                                  });
        }
      });
}
}  // namespace detail

/**
 * @brief Creates REST server for the target object on specified URI.
 * Server would handle request according to the object client-server traits.
 */
template <ClientServerType Target>
auto MakeServerFor
    [[nodiscard]] (cpp::NnUp<Target> target, Uri<Target> uri,
                   cpp::NnUp<network::IRestRequestReceiver> request_receiver) {
  auto server_builder =
      network::RestServerBuilder{std::move(uri), std::move(request_receiver)};
  auto shared_target = cpp::Share(std::move(target));

  detail::SetEndpointHandlers(server_builder, shared_target);
  return server_builder.Start();
}
}  // namespace vh::networkx

#endif  // VH_NETWORKX_MAKE_SERVER_FOR_H_
