#ifndef STONKS_NETWORK_NETWORK_REST_CLIENT_REQUEST_BUILDER_H_
#define STONKS_NETWORK_NETWORK_REST_CLIENT_REQUEST_BUILDER_H_

#include <memory>
#include <string>
#include <string_view>

#include "expose_private_constructor_to.h"
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
  auto WithBody(Parsable auto &&value) -> RequestBuilder & {
    return WithBody(ConvertToJson(std::forward<decltype(value)>(value)));
  }

  /**
   * @brief Sends the request discarding result.
   */
  void DiscardingResult() const;

  /**
   * @brief Sends the request and converts result to the specified type.
   */
  template <typename T>
  [[nodiscard]] auto AndReceive() -> T {
    return ParseFromJson<T>(*SendRequestAndGetResponse());
  }

 private:
  friend class utils::ExposePrivateConstructorsTo<RestClient, RequestBuilder>;

  explicit RequestBuilder(
      Endpoint endpoint,
      cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender);

  [[nodiscard]] auto WithParam(std::string_view key, Param value)
      -> RequestBuilder &;

  [[nodiscard]] auto WithBody(Body::value_type body) -> RequestBuilder &;

  [[nodiscard]] auto SendRequestAndGetResponse() const -> Result::value_type;

  Endpoint endpoint_{};
  RestRequestData data_{};
  cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender_;
};
}  // namespace rest_client
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_CLIENT_REQUEST_BUILDER_H_
