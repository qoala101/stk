#include "service_fallback.h"

#include "network_exception.h"

namespace stonks::service {
// NOLINTNEXTLINE(*-convert-member-functions-to-static)
auto Fallback::HandleNotSupportedUri() const -> cppcoro::task<> {
  throw network::Exception{"URI is not supported"};
}
}  // namespace stonks::service