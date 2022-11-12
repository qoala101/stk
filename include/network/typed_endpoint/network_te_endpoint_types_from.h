#ifndef STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TE_ENDPOINT_TYPES_FROM_H_
#define STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TE_ENDPOINT_TYPES_FROM_H_

#include <callable.hpp>
#include <gsl/assert>
#include <type_traits>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "network_typed_endpoint.h"

namespace stonks::network::te {
/**
 * @brief Passing the instance to API instead of parameter name
 * would make it to parse parameter as body.
 */
struct Body {};

namespace detail {
template <cpp::MemberFunction Function>
auto ParseResultType [[nodiscard]] () {
  using ReturnType = typename member_function_traits<Function>::return_type;

  if constexpr (std::is_same_v<ReturnType, void>) {
    return ParseTypeCheck{};
  } else {
    return ExpectedType<ReturnType>();
  }
}

template <typename T>
struct ParamTypeTraits {
  using ExpectedType = std::remove_const_t<T>;
};

template <typename T>
struct ParamTypeTraits<const T &> {
  using ExpectedType = T;
};

template <typename T>
struct ParamTypeTraits<const T *> {
  using ExpectedType = cpp::Opt<T>;
};

template <cpp::MemberFunction Function, int ParamIndex>
auto ParseParamType [[nodiscard]] () {
  using ParamType = typename member_function_traits<
      Function>::template argument_type<ParamIndex>;

  return ExpectedType<typename ParamTypeTraits<ParamType>::ExpectedType>();
}

template <cpp::MemberFunction Function, typename Param, typename... Params>
void ParseNextParamType(EndpointTypes &endpoint_types, Param &&param,
                        Params &&...params) {
  static constexpr auto kNumRemainingParams = sizeof...(Params);
  static constexpr auto kParamIndex =
      member_function_traits<Function>::argc - kNumRemainingParams - 1;

  auto param_type = ParseParamType<Function, kParamIndex>();

  if constexpr (const auto is_param_body =
                    std::is_same_v<std::decay_t<Param>, Body>) {
    Expects(endpoint_types.body.empty());
    endpoint_types.body = std::move(param_type);
  } else {
    Expects(!endpoint_types.params.contains(std::forward<Param>(param)));
    endpoint_types.params.emplace(std::forward<Param>(param),
                                  std::move(param_type));
  }

  if constexpr (kNumRemainingParams > 0) {
    ParseNextParamType<Function, Params...>(endpoint_types,
                                            std::forward<Params>(params)...);
  }
}

template <cpp::MemberFunction Function, typename... Params>
void ParseParamsAndBodyTypes(EndpointTypes &endpoint_types,
                             Params &&...params) {
  static constexpr auto kNumParams = sizeof...(Params);
  static_assert(kNumParams == member_function_traits<Function>::argc);

  if constexpr (kNumParams > 0) {
    detail::ParseNextParamType<Function, Params...>(
        endpoint_types, std::forward<Params>(params)...);
  }
}
}  // namespace detail

/**
 * @brief Parses endpoint types from the function.
 * @param params Names of function parameters. Could include the body tag
 * which tells that parameter is body.
 */
template <cpp::MemberFunction auto function, typename... Params>
auto EndpointTypesFrom [[nodiscard]] (Params &&...params) {
  using FunctionType = decltype(function);

  auto endpoint_types =
      EndpointTypes{.result = detail::ParseResultType<FunctionType>()};

  detail::ParseParamsAndBodyTypes<FunctionType, Params...>(
      endpoint_types, std::forward<Params>(params)...);

  return endpoint_types;
}
}  // namespace stonks::network::te

#endif  // STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TE_ENDPOINT_TYPES_FROM_H_
