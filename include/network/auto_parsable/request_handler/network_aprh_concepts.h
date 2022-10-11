#ifndef STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_CONCEPTS_H_
#define STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_CONCEPTS_H_

#include <concepts>

#include "cpp_concepts.h"      // IWYU pragma: keep
#include "network_concepts.h"  // IWYU pragma: keep

/**
 * @file Concepts used by auto-parsable request handler.
 */

namespace stonks::network::aprh {
template <typename T>
concept ConvertibleInvocable = requires(T &t) {
                                 { t() } -> Convertible;
                               };

template <typename T, typename... Args>
concept ConvertibleInvocableTaking =
    requires(T &t, Args &&...args) {
      { t(std::forward<Args>(args)...) } -> Convertible;
    } && cpp::NonVoidInvocableTaking<T, Args...>;
}  // namespace stonks::network::aprh

#endif  // STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_CONCEPTS_H_
