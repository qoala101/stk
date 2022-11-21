#ifndef STONKS_NETWORKX_NETWORKX_ENDPOINT_FUNCTION_TRAITS_FACADE_H_
#define STONKS_NETWORKX_NETWORKX_ENDPOINT_FUNCTION_TRAITS_FACADE_H_

#include <gsl/assert>
#include <nameof.hpp>
#include <string_view>
#include <type_traits>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_optional.h"
#include "member_function.hpp"
#include "network_typed_endpoint.h"
#include "network_types.h"
#include "networkx_concepts.h"  // IWYU pragma: keep
#include "networkx_types.h"

namespace stonks::networkx {
namespace detail {
auto ToUri [[nodiscard]] (std::string_view function_name) -> network::Uri;

template <typename T>
struct FunctionArgTypeTraits {
  using ExpectedType = std::remove_cvref_t<T>;
};

template <typename T>
struct FunctionArgTypeTraits<T *> {
  using ExpectedType = cpp::Opt<std::remove_cvref_t<T>>;
};
}  // namespace detail

/**
 * @brief Convenience API over endpoint function traits.
 */
template <cpp::MemberFunction auto kFunction,
          typename FunctionTraits = EndpointFunctionTraits<kFunction>>
  requires EndpointFunction<kFunction>
struct EndpointFunctionTraitsFacade : public FunctionTraits {
  using FunctionType = decltype(kFunction);

  /**
   * @brief Tells whether function has params.
   */
  static constexpr auto HasParams [[nodiscard]] () {
    return requires { FunctionTraits::kParams; };
  }

  /**
   * @brief Gives number of function params.
   */
  static constexpr auto GetNumParams [[nodiscard]] () {
    return std::tuple_size_v<decltype(FunctionTraits::kParams)>;
  }

  /**
   * @brief Gives endpoint function param by index.
   */
  template <unsigned kIndex>
  static constexpr auto GetParam [[nodiscard]] () -> auto & {
    return std::get<kIndex>(FunctionTraits::kParams);
  }

  /**
   * @brief Creates typed endpoint using provided traits.
   */
  static auto AsTypedEndpoint [[nodiscard]] () {
    return network::TypedEndpoint{
        .endpoint = {.method = FunctionTraits::kMethod,
                     .uri = detail::ToUri(nameof::nameof_member<kFunction>())},
        .expected_types = GetExpectedTypes()};
  }

 private:
  static auto GetExpectedResultType [[nodiscard]] () {
    using ResultType =
        typename member_function_traits<FunctionType>::return_type;

    if constexpr (std::is_same_v<ResultType, void>) {
      return network::ParseTypeCheck{};
    } else {
      return network::ExpectedType<ResultType>();
    }
  }

  template <unsigned kParamIndex>
  static void AddNextExpectedParamType(network::EndpointTypes &endpoint_types) {
    using FunctionArgType = typename member_function_traits<
        FunctionType>::template argument_type<kParamIndex>;

    auto expected_param_type =
        network::ExpectedType<typename detail::FunctionArgTypeTraits<
            FunctionArgType>::ExpectedType>();

    const auto param = GetParam<kParamIndex>();

    using ParamType = std::remove_cvref_t<decltype(param)>;

    if constexpr (const auto is_body = std::is_same_v<ParamType, RequestBody>) {
      Expects(endpoint_types.body.empty());
      endpoint_types.body = std::move(expected_param_type);
    } else {
      Expects(!endpoint_types.params.contains(param));
      endpoint_types.params.emplace(param, std::move(expected_param_type));
    }

    const auto num_remaining_params = GetNumParams() - kParamIndex - 1;

    if constexpr (num_remaining_params > 0) {
      AddNextExpectedParamType<kParamIndex - 1>(endpoint_types);
    }
  }

  static auto GetExpectedTypes [[nodiscard]] () {
    auto endpoint_types =
        network::EndpointTypes{.result = GetExpectedResultType()};

    if constexpr (HasParams()) {
      AddNextExpectedParamType<GetNumParams() - 1>(endpoint_types);
    }

    return endpoint_types;
  }
};
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_ENDPOINT_FUNCTION_TRAITS_FACADE_H_
