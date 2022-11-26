#ifndef STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_CONCEPTS_H_
#define STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_CONCEPTS_H_

#include <concepts>
#include <cppcoro/task.hpp>

#include "cpp_concepts.h"      // IWYU pragma: keep
#include "network_concepts.h"  // IWYU pragma: keep

/**
 * @file Concepts used by auto-parsable request handler.
 */

namespace stonks::network::aprh {
template <typename T, typename... Args>
concept ConvertibleInvocableTaking =
    Convertible<typename decltype(std::declval<T>()(
        std::declval<Args>()...))::value_type>;

template <typename T>
concept ConvertibleInvocable = ConvertibleInvocableTaking<T>;
}  // namespace stonks::network::aprh

#endif  // STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_CONCEPTS_H_
