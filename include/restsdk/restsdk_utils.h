#ifndef STONKS_RESTSDK_RESTSDK_UTILS_H_
#define STONKS_RESTSDK_RESTSDK_UTILS_H_

#include <cpprest/http_msg.h>

#include "network_enums.h"

namespace stonks::restsdk::utils {
[[nodiscard]] auto MethodFromNetworkMethod(network::Method method)
    -> web::http::method;
}  // namespace stonks::restsdk::utils

#endif  // STONKS_RESTSDK_RESTSDK_UTILS_H_
