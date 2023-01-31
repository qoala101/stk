#ifndef VH_NGROK_TYPES_H_
#define VH_NGROK_TYPES_H_

#include <string>

#include "cpp_typed_struct.h"

namespace vh::ngrok {
struct Tunnel {
  std::string public_url;
};

struct Tunnels : public cpp::TypedStruct<std::vector<Tunnel>> {};
}  // namespace vh::ngrok

#endif  // VH_NGROK_TYPES_H_
