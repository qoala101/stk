#ifndef STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_CONCEPTS_H_
#define STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_CONCEPTS_H_

#include <concepts>

#include "network_auto_parsable_request.h"

/**
 * @file Concepts used by auto-parsable request handler.
 */

namespace stonks::network::aprh {
template <typename T>
concept Callable = requires(T &t) {
                     { t() } -> std::same_as<void>;
                   };

template <typename T>
concept CallableWithRequest = requires(T &t, AutoParsableRestRequest request) {
                                { t(std::move(request)) } -> std::same_as<void>;
                              };

template <typename T>
concept CallableWithResponse = requires(T &t) {
                                 { t() } -> Convertible;
                               };

template <typename T>
concept CallableWithRequestAndResponse =
    requires(T &t, AutoParsableRestRequest request) {
      { t(std::move(request)) } -> Convertible;
    };
}  // namespace stonks::network::aprh

#endif  // STONKS_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_NETWORK_APRH_CONCEPTS_H_
