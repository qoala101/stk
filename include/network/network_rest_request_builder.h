#ifndef STONKS_NETWORK_NETWORK_REST_REQUEST_BUILDER_H_
#define STONKS_NETWORK_NETWORK_REST_REQUEST_BUILDER_H_

#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "network_enums.h"
#include "network_json_basic_conversions.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Utility to build data for REST request.
 */
class RestRequestBuilder {
 public:
  /**
   * @remark Method is GET by default.
   */
  auto WithMethod(Method method) -> RestRequestBuilder &;

  /**
   * @brief Set beginning of URI which usually starts with http://.
   */
  auto WithBaseUri(std::string base_uri) -> RestRequestBuilder &;

  /**
   * @brief Appends one more string to the base URI separated with /.
   */
  auto AppendUri(std::string_view uri) -> RestRequestBuilder &;

  /**
   * @brief Adds parameter to be send by request.
   * @remark Would override existing value with the same key.
   */
  auto AddParam(std::string key, Param value) -> RestRequestBuilder &;

  /**
   * @copydoc RestRequestBuilder::AddParam
   */
  auto AddParam(std::string key, auto &&value) -> RestRequestBuilder & {
    return AddParam(key, ConvertToJson(std::forward<decltype(value)>(value)));
  }

  /**
   * @brief Adds header to be send by request.
   * @remark Would override existing value with the same key.
   */
  auto AddHeader(std::string key, std::string value) -> RestRequestBuilder &;

  /**
   * @brief Sets the JSON body of the request.
   * @remark Should be called once.
   */
  auto WithBody(Body::value_type body) -> RestRequestBuilder &;

  /**
   * @brief Build REST request parts from inputs.
   */
  [[nodiscard]] auto Build() const & -> RestRequest;

  /**
   * @copydoc Build
   */
  [[nodiscard]] auto Build() && -> RestRequest;

 private:
  RestRequest request_{};
  bool uri_is_set_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_REQUEST_BUILDER_H_
