#ifndef STONKS_DI_DI_BIND_TYPE_TO_FACTORY_FUNCTION_H_
#define STONKS_DI_DI_BIND_TYPE_TO_FACTORY_FUNCTION_H_

#include <boost/di.hpp>
#include <type_traits>

namespace stonks::di {
namespace detail {
template <typename Type, typename FactoryFunction, typename... Args>
  requires std::is_invocable_r_v<Type, FactoryFunction, Args...>
struct FactoryFunctionInjector : public Type {
  explicit FactoryFunctionInjector(Args... args)
      : Type{FactoryFunction{}(std::move(args)...)} {}
};
}  // namespace detail

/**
 * @brief Binds type to the factory function.
 * @tparam FactoryFunction Callable which accepts list of Args.
 * Can accept by both, value and reference.
 * @tparam Args Arguments used by the factory function.
 * Typically those would be factory and arguments to its create method.
 * @remark Can be used for types which don't have public constructors.
 */
template <typename Type, typename FactoryFunction, typename... Args>
  requires std::is_invocable_r_v<Type, FactoryFunction, Args...>
[[nodiscard]] auto BindTypeToFactoryFunction() {
  return boost::di::bind<Type>()
      .template to<
          detail::FactoryFunctionInjector<Type, FactoryFunction, Args...>>();
}
}  // namespace stonks::di

#endif  // STONKS_DI_DI_BIND_TYPE_TO_FACTORY_FUNCTION_H_
