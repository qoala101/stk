/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_APRH_CONCEPTS_H_
#define VH_NETWORK_APRH_CONCEPTS_H_

#include <concepts>
#include <cppcoro/task.hpp>

#include "cpp_concepts.h"      // IWYU pragma: keep
#include "network_concepts.h"  // IWYU pragma: keep

/**
 * @file Concepts used by auto-parsable handlers.
 */

namespace vh::network::aprh {
template <typename T>
concept VoidCallable = cpp::CallableReturning<T, cppcoro::task<>>;

template <typename T, typename U>
concept VoidCallableTaking =
    cpp::CallableReturningTaking<T, cppcoro::task<>, U>;

template <typename T, typename... Args>
concept ConvertibleCallableTaking =
    Convertible<typename decltype(std::declval<T>()(
        std::declval<Args>()...))::value_type>;

template <typename T>
concept ConvertibleCallable = ConvertibleCallableTaking<T>;
}  // namespace vh::network::aprh

#endif  // VH_NETWORK_APRH_CONCEPTS_H_
