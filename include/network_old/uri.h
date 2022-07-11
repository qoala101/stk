#ifndef STONKS_NETWORK_URI_H_
#define STONKS_NETWORK_URI_H_

#include <string>
#include <string_view>

namespace stonks::network {
class PlainUri {
 public:
  [[nodiscard]] auto GetFullUri() const -> const std::string &;

 protected:
  explicit PlainUri(std::string_view full_uri);

 private:
  std::string full_uri_{};
};

class LocalUri : public PlainUri {
 public:
  explicit LocalUri(int port, std::string_view base_uri = "");
};

class ExternalUri : public PlainUri {
 public:
  explicit ExternalUri(std::string_view full_uri);
};

class Uri : public PlainUri {
 public:
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  Uri(LocalUri uri);
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  Uri(ExternalUri uri);
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_URI_H_
