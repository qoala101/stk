#include "network_i_rest_request_sender.h"

#include <tuple>

namespace stonks::network {
void IRestRequestSender::SendRequest(const RestRequestData &data) const {
  std::ignore = SendRequestAndGetResponse(data);
}
}  // namespace stonks::network