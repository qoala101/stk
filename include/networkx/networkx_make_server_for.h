#ifndef STONKS_NETWORKX_NETWORKX_MAKE_SERVER_FOR_H_
#define STONKS_NETWORKX_NETWORKX_MAKE_SERVER_FOR_H_

#include <tuple>
#include <type_traits>
#include <utility>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_not_null.h"
#include "network_auto_parsable_request.h"
#include "network_rest_server_builder.h"
#include "networkx_client_server_type_traits_facade.h"
#include "networkx_concepts.h"  // IWYU pragma: keep
#include "networkx_endpoint_function_traits_facade.h"

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

template <ClientServerType Target>
class SetEndpointHandlers {
 private:
  using TargetTraits = ClientServerTypeTraitsFacade<Target>;

  template <cpp::MemberFunction auto kFunction>
    requires EndpointFunction<kFunction> &&
             cpp::MemberFunctionOf<decltype(kFunction), Target>
  static auto InvokeWithRequestParams
      [[nodiscard]] (Target &target,
                     network::AutoParsableRestRequest &request) {
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

  template <unsigned kEndpointIndex>
  void AddNextEndpointHandler() const {
    constexpr auto function =
        TargetTraits::template GetEndpointFunction<kEndpointIndex>();

    server_builder_->Handling(
        EndpointFunctionTraitsFacade<function>::AsTypedEndpoint(),
        [target = target_](auto request) {
          return InvokeWithRequestParams<function>(*target, request);
        });

    if constexpr (kEndpointIndex > 0) {
      AddNextEndpointHandler<kEndpointIndex - 1>();
    }
  }

 public:
  SetEndpointHandlers(cpp::Nn<network::RestServerBuilder *> server_builder,
                      cpp::NnSp<Target> target)
      : server_builder_{server_builder}, target_{std::move(target)} {}

  void operator()() const {
    AddNextEndpointHandler<TargetTraits::GetNumEndpoints() - 1>();
  }

 private:
  cpp::Nn<network::RestServerBuilder *> server_builder_;
  cpp::NnSp<Target> target_;
};
}  // namespace detail

/**
 * @brief Creates REST server for the target object on specified URI.
 * Server would handle request according to the object client-server traits.
 */
template <ClientServerType Target>
auto MakeServerFor [[nodiscard]] (const cpp::NnSp<Target> &target,
                                  network::RestServerBuilder &server_builder) {
  detail::SetEndpointHandlers{cpp::AssumeNn(&server_builder), target}();
  return server_builder.Start();
}
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_MAKE_SERVER_FOR_H_
