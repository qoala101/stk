#ifndef STONKS_NETWORK_NETWORK_REST_CLIENT_REQUEST_BUILDER_H_
#define STONKS_NETWORK_NETWORK_REST_CLIENT_REQUEST_BUILDER_H_

#include <memory>
#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>

#include "ccutils_expose_private_constructors.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_rest_request_sender.h"
#include "network_json_basic_conversions.h"
#include "network_types.h"
#include "not_null.hpp"

namespace stonks::network {
class RestClient;

namespace rest_client {
/**
 * @brief Convenience API for building of requests from the REST client.
 */
class RequestBuilder {
 public:
  /**
   * @brief Sets request parameter.
   * @remark Overrides the parameter with the same name.
   */
  auto WithParam(std::string_view key, Convertible auto &&value)
      -> RequestBuilder & {
    return WithParam(key, ConvertToJson(std::forward<decltype(value)>(value)));
  }

  /**
   * @brief Sets the body of the request.
   * @remark Can only be called once.
   */
  auto WithBody(Convertible auto &&value) -> RequestBuilder & {
    return WithBody(ConvertToJson(std::forward<decltype(value)>(value)));
  }

  /**
   * @brief Sends the request discarding result.
   */
  void DiscardingResult() const &;

  /**
   * @copydoc DiscardingResult
   */
  void DiscardingResult() &&;

  /**
   * @brief Sends the request and converts result to the specified type.
   */
  template <Parsable T>
  [[nodiscard]] auto AndReceive() const & -> T {
    return ParseFromJson<T>(*SendRequestAndGetResult());
  }

  /**
   * @copydoc AndReceive
   */
  template <Parsable T>
  [[nodiscard]] auto AndReceive() && -> T {
    return ParseFromJson<T>(*std::move(*this).SendRequestAndGetResult());
  }

 private:
  friend class ccutils::ExposePrivateConstructorsTo<RestClient, RequestBuilder>;

  explicit RequestBuilder(
      Endpoint endpoint,
      cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender);

  static void DiscardingResultImpl(auto &&t);

  [[nodiscard]] static auto SendRequestAndGetResultImpl(auto &&t)
      -> Result::value_type;

  [[nodiscard]] auto WithParam(std::string_view key, Param value)
      -> RequestBuilder &;

  [[nodiscard]] auto WithBody(Body::value_type body) -> RequestBuilder &;

  [[nodiscard]] auto SendRequestAndGetResult() const & -> Result::value_type;
  [[nodiscard]] auto SendRequestAndGetResult() && -> Result::value_type;

  RestRequest request_{};
  cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender_;
};
}  // namespace rest_client
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_CLIENT_REQUEST_BUILDER_H_
