#ifndef STONKS_SERVICES_V2_INFO_SERVER_H_
#define STONKS_SERVICES_V2_INFO_SERVER_H_

#include "stonks_info.h"

namespace stonks {
class InfoServer {
 public:
  explicit InfoServer(std::string_view base_uri);

 private:
  network::Server server_;
  Info info_{};
};
}  // namespace stonks

#endif  // STONKS_SERVICES_V2_INFO_SERVER_H_
