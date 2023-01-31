#ifndef VH_NETWORKX_ENDPOINT_FUNCTION_TRAITS_FACADE_H_
#define VH_NETWORKX_ENDPOINT_FUNCTION_TRAITS_FACADE_H_

#include <callable.hpp>  // IWYU pragma: keep
#include <gsl/assert>
#include <member_function.hpp>
#include <nameof.hpp>
#include <string_view>
#include <type_traits>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_for_each_index.h"
#include "cpp_optional.h"
#include "network_typed_endpoint.h"
#include "network_types.h"
#include "networkx_concepts.h"  // IWYU pragma: keep
#include "networkx_types.h"

namespace vh::networkx {
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
  using NativeTraits = member_function_traits<FunctionType>;
  using ResultType = typename NativeTraits::return_type::value_type;

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
   * @brief Tells whether function is const.
   */
  static constexpr auto IsConst [[nodiscard]] () {
    return NativeTraits::is_const;
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
    if constexpr (std::is_same_v<ResultType, void>) {
      return network::ParseTypeCheck{};
    } else {
      return network::ExpectedType<ResultType>();
    }
  }

  static auto GetExpectedTypes [[nodiscard]] () {
    auto endpoint_types =
        network::EndpointTypes{.result = GetExpectedResultType()};

    if constexpr (HasParams()) {
      cpp::ForEachIndex<GetNumParams()>([&endpoint_types]<typename Current>(
                                            Current) {
        using FunctionArgType = ArgType<kFunction, Current::kIndex>;

        auto expected_param_type =
            network::ExpectedType<typename detail::FunctionArgTypeTraits<
                FunctionArgType>::ExpectedType>();

        const auto param = GetParam<Current::kIndex>();

        using ParamType = std::remove_cvref_t<decltype(param)>;

        if constexpr (const auto is_body =
                          std::is_same_v<ParamType, RequestBody>) {
          Expects(endpoint_types.body.empty());
          endpoint_types.body = std::move(expected_param_type);
        } else {
          Expects(!endpoint_types.params.contains(param));
          endpoint_types.params.emplace(param, std::move(expected_param_type));
        }
      });
    }

    return endpoint_types;
  }
};
}  // namespace vh::networkx

#endif  // VH_NETWORKX_ENDPOINT_FUNCTION_TRAITS_FACADE_H_
