#ifndef STONKS_NGROK_NGROK_TYPES_H_
#define STONKS_NGROK_NGROK_TYPES_H_

#include <string>

#include "cpp_typed_struct.h"

namespace vh::ngrok {
struct Tunnel {
  std::string public_url;
};

struct Tunnels : public cpp::TypedStruct<std::vector<Tunnel>> {};
}  // namespace vh::ngrok

#endif  // STONKS_NGROK_NGROK_TYPES_H_
