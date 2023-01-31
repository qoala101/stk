#ifndef STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_CONCEPTS_H_
#define STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_CONCEPTS_H_

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

#endif  // STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_CONCEPTS_H_
