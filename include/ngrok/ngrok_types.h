#ifndef STONKS_NGROK_NGROK_TYPES_H_
#define STONKS_NGROK_NGROK_TYPES_H_

#include <string>

namespace stonks::ngrok {
struct Tunnel {
  std::string public_url{};
} __attribute__((aligned(32)));  // NOLINT(*-magic-numbers)
}  // namespace stonks::ngrok

#endif  // STONKS_NGROK_NGROK_TYPES_H_
