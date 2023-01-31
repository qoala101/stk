#ifndef STONKS_CPP_CPP_CONCEPTS_H_
#define STONKS_CPP_CPP_CONCEPTS_H_

#include <callable.hpp>
#include <member_function.hpp>
#include <type_traits>
#include <variant>
#include <vector>

#include "cpp_optional.h"
#include "cpp_type_list.h"

/**
 * @file Common concepts.
 */

namespace stonks::cpp {
namespace detail {
template <typename T>
constexpr auto IsTypeList = false;

template <typename... Ts>
constexpr auto IsTypeList<TypeList<Ts...>> = true;

template <typename T, typename Ts>
constexpr auto IsVariantOf = false;

template <typename T, typename... Ts>
constexpr auto IsVariantOf<T, std::variant<Ts...>> =
    std::disjunction<std::is_same<T, Ts>...>::value;
}  // namespace detail

template <typename T>
struct TypedStruct;

template <typename T>
concept Const = std::is_const_v<std::remove_reference_t<T>>;

template <typename T>
concept Reference = std::is_lvalue_reference_v<T>;

template <typename T>
concept RawPointer = std::is_pointer_v<T>;

template <typename T>
concept PointerLike = requires(T t) {
                        t.operator->();
                        t.operator*();
                      };

template <typename T>
concept Rvalue = std::is_rvalue_reference_v<T>;

template <typename T, typename U>
concept Is = std::same_as<std::remove_cvref_t<T>, U>;

template <typename T>
concept Optional = std::same_as<T, Opt<typename T::value_type>>;

template <typename T>
concept Variant =
    requires { std::declval<std::variant_alternative_t<0, T>>(); };

template <typename T, typename Variant>
concept VariantOf = detail::IsVariantOf<T, Variant>;

template <typename T>
concept IsTypedStruct =
    std::derived_from<T, TypedStruct<typename T::ValueType>>;

template <typename T>
concept IsTypeList = detail::IsTypeList<T>;

template <typename T>
concept Vector = std::same_as<T, std::vector<typename T::value_type>>;

template <typename T>
concept Enum = std::is_enum_v<T>;

template <typename T>
concept Interface = std::is_polymorphic_v<T>;

template <typename T, typename... Args>
concept ConstructibleFrom =
    requires(Args &&...args) { T{std::forward<Args>(args)...}; };

template <typename T, typename U>
concept AssignableFrom = requires(T t, U &&u) { t = std::forward<U>(u); };

template <typename T>
concept MemberFunction = std::is_member_function_pointer_v<T>;

template <typename T, typename Parent>
concept MemberFunctionOf =
    std::same_as<typename member_function_traits<T>::class_type, Parent>;

template <typename T, typename U>
concept CallableReturning = requires(T t) {
                              { t() } -> std::same_as<U>;
                            };

template <typename T>
concept VoidCallable = CallableReturning<T, void>;

template <typename T>
concept NonVoidCallable = requires(T t) { t(); } && !
VoidCallable<T>;

template <typename T, typename U, typename... Args>
concept CallableReturningTaking = requires(T t, Args &&...args) {
                                    {
                                      t(std::forward<Args>(args)...)
                                      } -> std::same_as<U>;
                                  };

template <typename T, typename... Args>
concept VoidCallableTaking = CallableReturningTaking<T, void, Args...>;

template <typename T, typename... Args>
concept NonVoidCallableTaking =
    requires(T t, Args &&...args) { t(std::forward<Args>(args)...); } && !
VoidCallableTaking<T, Args...>;
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_CONCEPTS_H_
