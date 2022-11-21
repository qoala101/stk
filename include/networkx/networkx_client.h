#ifndef STONKS_NETWORKX_NETWORKX_CLIENT_H_
#define STONKS_NETWORKX_NETWORKX_CLIENT_H_

#include <type_traits>
#include <utility>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "member_function.hpp"
#include "network_rest_client.h"
#include "network_rest_client_request_builder.h"
#include "networkx_concepts.h"  // IWYU pragma: keep
#include "networkx_endpoint_function_traits_facade.h"
#include "networkx_types.h"

namespace stonks::networkx {
namespace detail {
class ClientBase {
 public:
  explicit ClientBase(network::RestClient rest_client);

 protected:
  auto GetRestClient [[nodiscard]] () const -> const network::RestClient &;

 private:
  network::RestClient rest_client_;
};

template <cpp::MemberFunction auto kFunction>
  requires EndpointFunction<kFunction>
class CallImpl {
 private:
  using FunctionType = decltype(kFunction);
  using FunctionTraits = EndpointFunctionTraitsFacade<kFunction>;

  template <typename Arg, typename... Args>
  void SetParamFromNextArg(Arg &&arg, Args &&...args) {
    const auto num_params = FunctionTraits::GetNumParams();
    const auto num_remaining_args = sizeof...(Args);
    const auto param_index = num_params - num_remaining_args - 1;

    const auto param = FunctionTraits::template GetParam<param_index>();

    using ParamType = std::remove_cvref_t<decltype(param)>;

    if constexpr (const auto is_body = std::is_same_v<ParamType, RequestBody>) {
      request_builder_.WithBody(std::forward<Arg>(arg));
    } else {
      request_builder_.WithParam(param, std::forward<Arg>(arg));
    }

    if constexpr (num_remaining_args > 0) {
      SetParamFromNextArg(std::forward<Args>(args)...);
    }
  }

  auto ExecuteAndGetResult [[nodiscard]] () {
    using ResultType =
        typename member_function_traits<FunctionType>::return_type;

    if constexpr (std::is_same_v<ResultType, void>) {
      request_builder_.DiscardingResult();
      return;
    } else {
      return request_builder_.template AndReceive<ResultType>();
    }
  }

 public:
  explicit CallImpl(const network::RestClient &rest_client)
      : request_builder_{rest_client.Call(FunctionTraits::AsTypedEndpoint())} {}

  template <typename... Args>
  auto operator() [[nodiscard]] (Args &&...args) {
    if constexpr (FunctionTraits::HasParams()) {
      SetParamFromNextArg(std::forward<Args>(args)...);
    }

    return ExecuteAndGetResult();
  }

 private:
  network::rest_client::RequestBuilder request_builder_;
};
}  // namespace detail

/**
 * @brief Client for remote server with convenient API.
 */
template <ClientServerType Target>
class Client : public detail::ClientBase {
 public:
  using ClientBase::ClientBase;

  /**
   * @brief Remotely calls specified function with provided arguments.
   */
  template <cpp::MemberFunctionOf<Target> auto kFunction, typename... Args,
            typename FunctionType = decltype(kFunction)>
    requires EndpointFunction<kFunction> &&
             std::is_invocable_v<FunctionType, Target &, Args...>
  auto Call [[nodiscard]] (Args &&...args) const {
    return detail::CallImpl<kFunction>{GetRestClient()}(
        std::forward<Args>(args)...);
  }
};
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_CLIENT_H_
