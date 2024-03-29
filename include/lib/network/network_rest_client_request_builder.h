/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_REST_CLIENT_REQUEST_BUILDER_H_
#define VH_NETWORK_REST_CLIENT_REQUEST_BUILDER_H_

#include <absl/container/flat_hash_map.h>

#include <cppcoro/task.hpp>
#include <not_null.hpp>
#include <optional>
#include <string_view>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_this.h"          // IWYU pragma: keep
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_rest_request_sender.h"
#include "network_json_conversions_facades.h"
#include "network_types.h"

namespace vh::network::rest_client {
/**
 * @brief Convenience API for building requests from the REST client.
 */
class RequestBuilder {
 public:
  RequestBuilder(Endpoint endpoint,
                 cpp::NnUp<IRestRequestSender> request_sender);

  /**
   * @brief Sets request parameter.
   * @remark Overrides the parameter with the same name.
   */
  template <Convertible T>
  auto WithParam(std::string_view key, T &&value) -> auto & {
    return WithParam(key, ConvertToJson(std::forward<T>(value)));
  }

  /**
   * @brief Sets the body of the request.
   * @remark Can only be called once.
   */
  template <Convertible T>
  auto WithBody(T &&t) -> auto & {
    return WithBody(ConvertToJson(std::forward<T>(t)));
  }

  /**
   * @brief Sends the request discarding result.
   */
  auto DiscardingResult [[nodiscard]] () const -> cppcoro::task<>;

  /**
   * @remark Other methods should not be called after this.
   */
  auto DiscardingResult [[nodiscard]] () -> cppcoro::task<>;

  /**
   * @brief Sends the request and converts result to the specified type.
   */
  template <Parsable T>
  auto AndReceive [[nodiscard]] () const -> cppcoro::task<T> {
    co_return ParseFromJson<T>(*co_await SendRequestAndGetResult());
  }

  /**
   * @remark Other methods should not be called after this.
   */
  template <Parsable T>
  auto AndReceive [[nodiscard]] () -> cppcoro::task<T> {
    co_return ParseFromJson<T>(*co_await SendRequestAndGetResult());
  }

 private:
  static auto DiscardingResultImpl
      [[nodiscard]] (cpp::This<RequestBuilder> auto &t) -> cppcoro::task<>;

  template <Parsable T>
  static auto SendRequestAndGetResultImpl
      [[nodiscard]] (cpp::This<RequestBuilder> auto &t) -> cppcoro::task<T>;

  auto WithParam [[nodiscard]] (std::string_view key, Param value)
  -> RequestBuilder &;

  auto WithBody [[nodiscard]] (Body body) -> RequestBuilder &;

  auto SendRequestAndGetResult [[nodiscard]] () const -> cppcoro::task<Result>;
  auto SendRequestAndGetResult [[nodiscard]] () -> cppcoro::task<Result>;

  Endpoint endpoint_{};
  Params params_{};
  Headers headers_{};
  cpp::Opt<Body> body_{};
  cpp::NnUp<IRestRequestSender> request_sender_;
};
}  // namespace vh::network::rest_client

#endif  // VH_NETWORK_REST_CLIENT_REQUEST_BUILDER_H_
