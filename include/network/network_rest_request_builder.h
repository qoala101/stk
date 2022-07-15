#ifndef STONKS_NETWORK_NETWORK_REST_REQUEST_BUILDER_H_
#define STONKS_NETWORK_NETWORK_REST_REQUEST_BUILDER_H_

#include <polymorphic_value.h>

#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "concepts.h"  // IWYU pragma: keep
#include "network_enums.h"
#include "network_i_json.h"
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
  auto WithBaseUri(std::string_view base_uri) -> RestRequestBuilder &;

  /**
   * @brief Appends one more string to the base URI separated with /.
   */
  auto AppendUri(std::string_view uri) -> RestRequestBuilder &;

  /**
   * @brief Adds parameter to be send by request.
   * @remark Would override existing value with the same key.
   */
  auto AddParam(std::string_view key,
                isocpp_p0201::polymorphic_value<IJson> value)
      -> RestRequestBuilder &;

  /**
   * @copydoc RestRequestBuilder::AddParam
   */
  template <typename T>
  auto AddParam(std::string_view key, T &&value) -> RestRequestBuilder & {
    return AddParam(key, ConvertToJson(std::forward<T>(value)));
  }

  /**
   * @brief Adds header to be send by request.
   * @remark Would override existing value with the same key.
   */
  auto AddHeader(std::string_view key, std::string_view value)
      -> RestRequestBuilder &;

  /**
   * @brief Sets the JSON body of the request.
   * @remark Should be called once.
   */
  auto WithBody(isocpp_p0201::polymorphic_value<IJson> body)
      -> RestRequestBuilder &;

  /**
   * @brief Build REST request parts from inputs.
   */
  [[nodiscard]] auto Build() const & -> std::pair<Endpoint, RestRequestData>;

  /**
   * @copydoc Build
   */
  [[nodiscard]] auto Build() && -> std::pair<Endpoint, RestRequestData>;

 private:
  Method method_{};
  std::optional<std::string> uri_{};
  std::map<std::string, isocpp_p0201::polymorphic_value<IJson>> params_{};
  std::map<std::string, std::string> headers_{};
  std::optional<isocpp_p0201::polymorphic_value<IJson>> body_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_REQUEST_BUILDER_H_
