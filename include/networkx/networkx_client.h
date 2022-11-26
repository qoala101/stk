#ifndef STONKS_NETWORKX_NETWORKX_CLIENT_H_
#define STONKS_NETWORKX_NETWORKX_CLIENT_H_

#include <callable.hpp>
#include <type_traits>
#include <utility>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_for_each_arg.h"
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
  using FunctionTraits = EndpointFunctionTraitsFacade<kFunction>;

  auto ExecuteAndGetResult [[nodiscard]] () {
    using ResultType = typename member_function_traits<
        decltype(kFunction)>::return_type::value_type;

    if constexpr (std::is_same_v<ResultType, void>) {
      return request_builder_.DiscardingResult();
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
      cpp::ForEachArg(
          [&request_builder = request_builder_]<typename Arg, typename Current>(
              Arg &&arg, Current) {
            const auto param =
                FunctionTraits::template GetParam<Current::kIndex>();

            using ParamType = std::remove_cvref_t<decltype(param)>;

            if constexpr (const auto is_body =
                              std::is_same_v<ParamType, RequestBody>) {
              request_builder.WithBody(std::forward<Arg>(arg));
            } else {
              request_builder.WithParam(param, std::forward<Arg>(arg));
            }
          },
          std::forward<Args>(args)...);
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
             std::invocable<FunctionType, Target &, Args...>
  auto Call [[nodiscard]] (Args &&...args) const {
    return detail::CallImpl<kFunction>{GetRestClient()}(
        std::forward<Args>(args)...);
  }
};
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_CLIENT_H_
