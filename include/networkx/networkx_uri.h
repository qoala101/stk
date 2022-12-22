#ifndef STONKS_NETWORKX_NETWORKX_URI_H_
#define STONKS_NETWORKX_NETWORKX_URI_H_

#include "network_types.h"
#include "networkx_concepts.h"

namespace stonks::networkx {
/**
 * @brief URI of specific type of server.
 */
template <ClientServerType Target>
struct Uri : public network::Uri {};
}  // namespace stonks::networkx

#endif  // STONKS_NETWORKX_NETWORKX_URI_H_
