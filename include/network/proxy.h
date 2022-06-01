#ifndef STONKS_NETWORK_PROXY_H_
#define STONKS_NETWORK_PROXY_H_

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
#include <cpprest/http_msg.h>

#include <map>
#include <shared_mutex>
#include <string_view>

namespace stonks::network {
/**
 * @brief Redirects all the stonks requests to the corresponding registered
 * endpoints.
 */
class ProxyInterface {  // NOLINT(*-special-member-functions)
 public:
  virtual ~ProxyInterface() = default;

  /**
   * @brief Returns port for the registered endpoint or nullopt if endpoint is
   * not registered.
   */
  [[nodiscard]] virtual auto GetEndpointPort(std::string_view endpoint) const
      -> std::optional<int> = 0;

  /**
   * @brief Adds endpoint to the map and sets the port for it. Later all the
   * requests which came to this endpoint, are going to be redirected to that
   * port.
   */
  virtual void RegisterEndpoint(std::string_view endpoint) = 0;
};

/**
 * @copydoc ProxyInterface
 */
class Proxy : public ProxyInterface {
 public:
  static constexpr auto kPort = 6507;

  /**
   * @brief Sets up the proxy at predefined port which is expected to be exposed
   * to the stonks users.
   */
  explicit Proxy();

  /**
   * @copydoc ProxyInterface::GetEndpointPort
   */
  [[nodiscard]] auto GetEndpointPort(std::string_view endpoint) const
      -> std::optional<int> override;

  /**
   * @copydoc ProxyInterface::RegisterEndpoint
   */
  void RegisterEndpoint(std::string_view endpoint) override;

 private:
  void HandleRequest(const web::http::http_request &request);

  web::http::experimental::listener::http_listener http_listener_{};
  std::map<std::string, web::http::client::http_client> endpoints_{};
  int last_available_port_{};
  mutable std::shared_mutex mutex_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_PROXY_H_
