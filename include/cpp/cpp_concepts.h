#ifndef STONKS_CPP_CPP_CONCEPTS_H_
#define STONKS_CPP_CPP_CONCEPTS_H_

#include <type_traits>
#include <vector>

#include "cpp_optional.h"

/**
 * @file Common concepts.
 */

namespace stonks::cpp {
template <typename T>
class TypedStruct;

template <typename T>
concept Const = std::is_const_v<std::remove_reference_t<T>>;

template <typename T>
concept Reference = std::is_lvalue_reference_v<T>;

template <typename T>
concept Rvalue = std::is_rvalue_reference_v<T>;

template <typename T, typename U>
concept DecaysTo = std::same_as<std::decay_t<T>, U>;

template <typename T>
concept Optional = std::same_as<T, Opt<typename T::value_type>>;

template <typename T>
concept IsTypedStruct =
    std::derived_from<T, TypedStruct<typename T::ValueType>>;

template <typename T>
concept Vector = std::same_as<T, std::vector<typename T::value_type>>;

template <typename T>
concept VoidInvocable = requires(T &t) {
                          { t() } -> std::same_as<void>;
                        };

template <typename T>
concept NonVoidInvocable = requires(T &t) { t(); } && !
VoidInvocable<T>;

template <typename T, typename... Args>
concept VoidInvocableTakes = requires(T &t, Args &&...args) {
                               {
                                 t(std::forward<Args>(args)...)
                                 } -> std::same_as<void>;
                             };

template <typename T, typename... Args>
concept NonVoidInvocableTakes =
    requires(T &t, Args &&...args) { t(std::forward<Args>(args)...); } && !
VoidInvocableTakes<T, Args...>;
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_CONCEPTS_H_
