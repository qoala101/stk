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
#include "networkx_types.h"
#include "networkx_web_socket_sender.h"

namespace stonks::networkx {
namespace detail {
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

template <cpp::MemberFunction auto kFunction>
constexpr auto HasConstCorrectMethod [[nodiscard]] () {
  const auto is_const_function =
      member_function_traits<decltype(kFunction)>::is_const;
  const auto is_get_method =
      EndpointFunctionTraits<kFunction>::kMethod == network::Method::kGet;
  return is_const_function == is_get_method;
}

template <typename T>
constexpr auto IsEndpointFunctions = false;

template <cpp::MemberFunction... Ts>
constexpr auto IsEndpointFunctions<std::tuple<Ts...>> = true;
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
concept EndpointFunction = detail::HasConstCorrectMethod<kFunction>() &&
                           detail::HasParamForEachArg<kFunction>();

/**
 * @brief Type for which client-server traits are defined.
 */
template <typename T>
concept ClientServerType = detail::IsEndpointFunctions<std::remove_cvref_t<
    decltype(ClientServerTypeTraits<T>::kEndpointFunctions)>>;

/**
 * @brief Function which can be used as web socket receiver.
 */
template <auto kFunction>
concept WebSocketReceiver =
    requires(ParentType<kFunction> parent) {
      {
        (parent.*kFunction)(std::declval<network::AutoParsable>())
        } -> std::same_as<cppcoro::task<>>;
    };

/**
 * @brief Function which can be used as web socket receiver with ability
 * to reply.
 */
template <auto kFunction>
concept WebSocketReceiverSender =
    (member_function_traits<decltype(kFunction)>::argc == 2) &&
    requires(ParentType<kFunction> parent) {
      {
        (parent.*kFunction)(
            std::declval<network::AutoParsable>(),
            std::declval<WebSocketSender<typename std::remove_cvref_t<
                ArgType<kFunction, 1>>::SendsType>>())
        } -> std::same_as<cppcoro::task<>>;
    };
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_CONCEPTS_H_
