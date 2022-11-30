#ifndef STONKS_NETWORKX_NETWORKX_CONCEPTS_H_
#define STONKS_NETWORKX_NETWORKX_CONCEPTS_H_

#include <callable.hpp>
#include <concepts>
#include <cppcoro/task.hpp>
#include <member_function.hpp>
#include <string>
#include <tuple>
#include <type_traits>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "network_auto_parsable.h"
#include "network_enums.h"
#include "network_ws_types.h"
#include "networkx_types.h"
#include "networkx_web_socket_sender.h"

namespace stonks::networkx {
namespace detail {
template <cpp::MemberFunction auto F>
using ArgType =
    typename member_function_traits<decltype(F)>::template argument_type<0>;

template <cpp::MemberFunction auto kFunction>
constexpr auto HasParamForEachArg [[nodiscard]] () {
  const auto num_args = member_function_traits<decltype(kFunction)>::argc;

  if constexpr (const auto has_params =
                    requires { EndpointFunctionTraits<kFunction>::kParams; }) {
    const auto num_params =
        std::tuple_size_v<decltype(EndpointFunctionTraits<kFunction>::kParams)>;
    return num_args == num_params;
  } else {
    return num_args == 0;
  }
}

template <typename T>
constexpr auto IsEndpointFunctions = false;

template <cpp::MemberFunction... Ts>
constexpr auto IsEndpointFunctions<std::tuple<Ts...>> = true;

template <cpp::MemberFunction auto F>
constexpr auto HasSingleArg = member_function_traits<decltype(F)>::argc == 1;
}  // namespace detail

/**
 * @brief Valid endpoint parameter type.
 */
template <typename T>
concept EndpointParam =
    std::convertible_to<T, std::string> || std::same_as<T, RequestBody>;

/**
 * @brief Function for which endpoint function traits are defined.
 */
template <auto kFunction>
concept EndpointFunction = requires {
                             {
                               EndpointFunctionTraits<kFunction>::kMethod
                               } -> cpp::Is<network::Method>;
                           } && detail::HasParamForEachArg<kFunction>();

/**
 * @brief Type for which client-server traits are defined.
 */
template <typename T>
concept ClientServerType = detail::IsEndpointFunctions<std::remove_cvref_t<
    decltype(ClientServerTypeTraits<T>::kEndpointFunctions)>>;

/**
 * @brief Web socket type with correctly declared Receive function.
 */
template <typename T>
concept ReceivesFromWebSocket =
    requires(T t) {
      {
        t.Receive(std::declval<network::AutoParsable>())
        } -> std::same_as<cppcoro::task<>>;
    };

/**
 * @brief Web socket type with correctly declared SetSender function.
 */
template <typename T>
concept SendsToWebSocket =
    requires(T t) {
      {
        t.SetSender(std::declval<WebSocketSender<
                        typename detail::ArgType<&T::SetSender>::SendsType>>())
        } -> std::same_as<void>;
    };

/**
 * @brief Type which can be used by Web Socket.
 * Must provide endpoint and optional Receive and SetSender members
 * for interaction with the socket.
 */
template <typename T>
concept WebSocketType = requires(T t) {
                          {
                            t.GetEndpoint()
                            } -> std::same_as<network::WsEndpoint>;
                        } && (ReceivesFromWebSocket<T> || SendsToWebSocket<T>);
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_CONCEPTS_H_
