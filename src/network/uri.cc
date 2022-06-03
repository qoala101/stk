#include "uri.h"

#include <type_traits>
#include <utility>

namespace stonks::network {
PlainUri::PlainUri(std::string_view full_uri) : full_uri_{full_uri} {}

auto PlainUri::GetFullUri() const -> const std::string& { return full_uri_; }

LocalUri::LocalUri(int port, std::string_view base_uri)
    : PlainUri{"http://localhost:" + std::to_string(port) + base_uri.data()} {}

ExternalUri::ExternalUri(std::string_view full_uri) : PlainUri{full_uri} {}

Uri::Uri(LocalUri uri) : PlainUri{std::move(uri)} {}

Uri::Uri(ExternalUri uri) : PlainUri{std::move(uri)} {}
}  // namespace stonks::network