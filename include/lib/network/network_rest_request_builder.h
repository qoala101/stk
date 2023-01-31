#ifndef VH_NETWORK_REST_REQUEST_BUILDER_H_
#define VH_NETWORK_REST_REQUEST_BUILDER_H_

#include <absl/container/flat_hash_map.h>

#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "cpp_optional.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_json_base_conversions.h"
#include "network_types.h"

namespace vh::network {
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
  auto WithBaseUri(Uri base_uri) -> RestRequestBuilder &;

  /**
   * @brief Appends one more string to the base URI separated with /.
   */
  auto AppendUri(const Uri &uri) -> RestRequestBuilder &;

  /**
   * @brief Adds parameter to be send by request.
   */
  auto AddParam(std::string_view key, Param value) -> RestRequestBuilder &;

  /**
   * @copydoc RestRequestBuilder::AddParam
   */
  template <Convertible T>
  auto AddParam(std::string_view key, T &&value) -> auto & {
    return AddParam(key, ConvertToJson(std::forward<T>(value)));
  }

  /**
   * @brief Adds header to be send by request.
   */
  auto AddHeader(std::string_view key, std::string value)
      -> RestRequestBuilder &;

  /**
   * @brief Sets the JSON body of the request.
   * @remark Should be called once.
   */
  auto WithBody(Body body) -> RestRequestBuilder &;

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
  cpp::Opt<Method> method_{};
  cpp::Opt<Uri> uri_{};
  Params params_{};
  Headers headers_{};
  cpp::Opt<Body> body_{};
};
}  // namespace vh::network

#endif  // VH_NETWORK_REST_REQUEST_BUILDER_H_
