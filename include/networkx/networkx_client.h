#ifndef STONKS_NETWORKX_NETWORKX_CLIENT_H_
#define STONKS_NETWORKX_NETWORKX_CLIENT_H_

#include <concepts>
#include <cppcoro/task.hpp>
#include <type_traits>
#include <utility>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_for_each_arg.h"
#include "network_rest_client.h"
#include "network_rest_client_request_builder.h"
#include "networkx_concepts.h"  // IWYU pragma: keep
#include "networkx_endpoint_function_traits_facade.h"
#include "networkx_types.h"
#include "networkx_uri.h"

namespace stonks::networkx {
namespace detail {
class ClientBase {
 protected:
  ClientBase(network::Uri &uri,
             di::Factory<network::IRestRequestSender> &request_sender_factory);

  auto GetRestClient [[nodiscard]] () const -> const network::RestClient &;

 private:
  network::RestClient rest_client_;
};

template <cpp::MemberFunction auto kFunction>
  requires EndpointFunction<kFunction>
class CallImpl {
 private:
  using FunctionTraits = EndpointFunctionTraitsFacade<kFunction>;
  using ResultType = typename FunctionTraits::ResultType;

  auto ExecuteAndGetResult [[nodiscard]] () -> cppcoro::task<ResultType> {
    if constexpr (std::is_same_v<ResultType, void>) {
      co_await request_builder_.DiscardingResult();
    } else {
      co_return co_await request_builder_.template AndReceive<ResultType>();
    }
  }

 public:
  explicit CallImpl(const network::RestClient &rest_client)
      : request_builder_{rest_client.Call(FunctionTraits::AsTypedEndpoint())} {}

  template <typename... Args>
  auto operator() [[nodiscard]] (Args &&...args) -> cppcoro::task<ResultType> {
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

    co_return co_await ExecuteAndGetResult();
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
  /**
   * @param uri Server URI.
   */
  Client(Uri<Target> uri,
         di::Factory<network::IRestRequestSender> request_sender_factory)
      : ClientBase{uri, request_sender_factory} {}

  /**
   * @brief Remotely calls specified function with provided arguments.
   */
  template <cpp::MemberFunctionOf<Target> auto kFunction, typename... Args,
            typename FunctionType = decltype(kFunction),
            typename ResultType =
                typename EndpointFunctionTraitsFacade<kFunction>::ResultType>
    requires EndpointFunction<kFunction> &&
             std::invocable<FunctionType, Target &, Args...>
             auto Call [[nodiscard]] (Args &&...args) const
             -> cppcoro::task<ResultType> {
    co_return co_await detail::CallImpl<kFunction>{GetRestClient()}(
        std::forward<Args>(args)...);
  }
};
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_CLIENT_H_
