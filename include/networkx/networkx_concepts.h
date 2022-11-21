#ifndef STONKS_NETWORKX_NETWORKX_CONCEPTS_H_
#define STONKS_NETWORKX_NETWORKX_CONCEPTS_H_

#include <callable.hpp>
#include <concepts>
#include <tuple>
#include <type_traits>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "network_enums.h"
#include "networkx_types.h"

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

template <typename T>
constexpr auto IsEndpointFunctions = false;

template <cpp::MemberFunction... Ts>
constexpr auto IsEndpointFunctions<std::tuple<Ts...>> = true;
}  // namespace detail

/**
 * @brief Function for which endpoint function traits are defined.
 */
template <auto kFunction>
concept EndpointFunction = requires {
                             {
                               EndpointFunctionTraits<kFunction>::kMethod
                               } -> cpp::DecaysTo<network::Method>;
                           } && detail::HasParamForEachArg<kFunction>();

/**
 * @brief Type for which client-server traits are defined.
 */
template <typename T>
concept ClientServerType = detail::IsEndpointFunctions<std::remove_cvref_t<
    decltype(ClientServerTypeTraits<T>::kEndpointFunctions)>>;
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_CONCEPTS_H_
