#ifndef STONKS_CPP_DI_CPP_DI_BIND_TYPE_TO_FACTORY_FUNCTION_H_
#define STONKS_CPP_DI_CPP_DI_BIND_TYPE_TO_FACTORY_FUNCTION_H_

#include <boost/di.hpp>

namespace stonks::cpp::di {
namespace detail {
template <typename Type, typename FactoryFunction>
struct FactoryFunctionFactory {
  [[nodiscard]] auto operator()(const auto &injector) const -> Type {
    return factory_function_(injector);
  }

  FactoryFunction factory_function_{};
};
}  // namespace detail

/**
 * @brief Binds type to the factory function.
 * @remark Can be used for types which don't have public constructors.
 * @remark Factory function accepts single injector argument,
 * which should be used to create all the entities inside the function.
 */
template <typename Type, typename FactoryFunction>
[[nodiscard]] auto BindTypeToFactoryFunction(FactoryFunction factory_function) {
  return boost::di::bind<Type>().to(
      detail::FactoryFunctionFactory<Type, FactoryFunction>{
          std::move(factory_function)});
}
}  // namespace stonks::cpp::di

#endif  // STONKS_CPP_DI_CPP_DI_BIND_TYPE_TO_FACTORY_FUNCTION_H_
