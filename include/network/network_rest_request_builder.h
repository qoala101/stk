#ifndef STONKS_NETWORK_NETWORK_REST_REQUEST_BUILDER_H_
#define STONKS_NETWORK_NETWORK_REST_REQUEST_BUILDER_H_

#include <string>
#include <utility>

#include "cpp_optional.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_json_base_conversions.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Utility to build data for REST request.
 */
class RestRequestBuilder {
 public:
  RestRequestBuilder();

  /**
   * @remark Method is GET by default.
   */
  auto WithMethod(Method method) -> RestRequestBuilder &;

  /**
   * @brief Set beginning of URI which usually starts with http://.
   */
  auto WithBaseUri(Uri base_uri) -> RestRequestBuilder &;

  /**
   * @brief Appends one more string to the base URI separated with /.
   */
  auto AppendUri(Uri uri) -> RestRequestBuilder &;

  /**
   * @brief Adds parameter to be send by request.
   * @remark Would override existing value with the same key.
   */
  auto AddParam(std::string key, Param value) -> RestRequestBuilder &;

  /**
   * @copydoc RestRequestBuilder::AddParam
   */
  template <Convertible T>
  auto AddParam(std::string key, T &&value) -> auto & {
    return AddParam(key, ConvertToJson(std::forward<T>(value)));
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
   * @copydoc RestRequestBuilder::WithBody
   */
  template <Convertible T>
  auto WithBody(T &&value) -> auto & {
    return WithBody(ConvertToJson(std::forward<T>(value)));
  }

  /**
   * @brief Build REST request parts from inputs.
   */
  auto Build [[nodiscard]] () const -> RestRequest;

  /**
   * @remark Other methods should not be called after this.
   */
  auto Build [[nodiscard]] () -> RestRequest;

 private:
  cpp::Opt<RestRequest> request_{};
  bool uri_is_set_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_REQUEST_BUILDER_H_
