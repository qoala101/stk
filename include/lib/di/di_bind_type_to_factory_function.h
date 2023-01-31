#ifndef STONKS_DI_DI_BIND_TYPE_TO_FACTORY_FUNCTION_H_
#define STONKS_DI_DI_BIND_TYPE_TO_FACTORY_FUNCTION_H_

#include <boost/di.hpp>
#include <callable.hpp>
#include <type_traits>

#include "cpp_concepts.h"  // IWYU pragma: keep

namespace vh::di {
namespace detail {
template <typename T, auto kFactoryFunction,
          typename FactoryFunction = decltype(kFactoryFunction)>
struct FactoryFunctionInjector;

template <typename T, auto kFactoryFunction, typename... Args>
  requires cpp::CallableReturningTaking<decltype(kFactoryFunction), T, Args...>
struct FactoryFunctionInjector<T, kFactoryFunction, T (*)(Args...)> : public T {
  explicit FactoryFunctionInjector(Args... args)
      : T{kFactoryFunction(std::move(args)...)} {}
};
}  // namespace detail

/**
 * @brief Binds type to the factory function.
 * @tparam kFactoryFunction Stateless callable which accepts list of arguments
 * to be injected.
 * @remark Can be used for types which don't have public constructors.
 * @remark Lambdas should be prepended with + before []
 * in order to treat them as function pointers.
 */
template <typename T, auto kFactoryFunction>
  requires std::same_as<
      typename callable_traits<decltype(kFactoryFunction)>::return_type, T>
auto BindTypeToFactoryFunction [[nodiscard]] () {
  return boost::di::bind<T>()
      .template to<detail::FactoryFunctionInjector<T, kFactoryFunction>>();
}
}  // namespace vh::di

#endif  // STONKS_DI_DI_BIND_TYPE_TO_FACTORY_FUNCTION_H_
