#ifndef STONKS_NETWORK_NETWORK_REST_CLIENT_REQUEST_BUILDER_H_
#define STONKS_NETWORK_NETWORK_REST_CLIENT_REQUEST_BUILDER_H_

#include <optional>
#include <string>
#include <utility>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_rest_request_sender.h"
#include "network_json_basic_conversions.h"
#include "network_types.h"

namespace stonks::network {
class RestClient;

namespace rest_client {
/**
 * @brief Convenience API for building requests from the REST client.
 */
class RequestBuilder {
 public:
  /**
   * @brief Sets request parameter.
   * @remark Overrides the parameter with the same name.
   */
  template <Convertible Value>
  auto WithParam(std::string key, Value &&value) -> RequestBuilder & {
    return WithParam(std::move(key), ConvertToJson(std::forward<Value>(value)));
  }

  /**
   * @brief Sets the body of the request.
   * @remark Can only be called once.
   */
  template <Convertible Value>
  auto WithBody(Value &&value) -> RequestBuilder & {
    return WithBody(ConvertToJson(std::forward<Value>(value)));
  }

  /**
   * @brief Sends the request discarding result.
   */
  void DiscardingResult() const;

  /**
   * @copydoc DiscardingResult
   * @remark Other methods should not be called after this.
   */
  void DiscardingResult();

  /**
   * @brief Sends the request and converts result to the specified type.
   */
  template <Parsable T>
  [[nodiscard]] auto AndReceive() const -> T {
    return ParseFromJson<T>(*SendRequestAndGetResult());
  }

  /**
   * @copydoc AndReceive
   * @remark Other methods should not be called after this.
   */
  template <Parsable T>
  [[nodiscard]] auto AndReceive() -> T {
    return ParseFromJson<T>(*SendRequestAndGetResult());
  }

 private:
  friend class cpp::ExposePrivateConstructorsTo<RestClient, RequestBuilder>;

  RequestBuilder(Endpoint endpoint,
                 cpp::NnSp<IRestRequestSender> request_sender);

  template <cpp::DecaysTo<RequestBuilder> This>
  static void DiscardingResultImpl(This &t);

  template <cpp::DecaysTo<RequestBuilder> This>
  [[nodiscard]] static auto SendRequestAndGetResultImpl(This &t)
      -> Result::value_type;

  [[nodiscard]] auto WithParam(std::string key, Param value)
      -> RequestBuilder &;

  [[nodiscard]] auto WithBody(Body::value_type body) -> RequestBuilder &;

  [[nodiscard]] auto SendRequestAndGetResult() const -> Result::value_type;
  [[nodiscard]] auto SendRequestAndGetResult() -> Result::value_type;

  RestRequest request_{};
  cpp::NnSp<IRestRequestSender> request_sender_;
};
}  // namespace rest_client
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_CLIENT_REQUEST_BUILDER_H_
