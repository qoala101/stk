#ifndef STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_
#define STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_

#include <function2/function2.hpp>
#include <utility>
#include <variant>

#include "network_auto_parsable_request.h"
#include "network_i_rest_request_handler.h"
#include "network_types.h"

namespace stonks::network {
namespace detail {
template <typename T>
concept Callable = requires(T &t) {
  { t() } -> std::same_as<void>;
};

template <typename T>
concept CallableWithRequest = requires(T &t, AutoParsableRestRequest request) {
  { t(std::move(request)) } -> std::same_as<void>;
};

template <typename T>
concept CallableWithResponse = requires(T &t) {
  { t() } -> Convertible;
};

template <typename T>
concept CallableWithRequestAndResponse =
    requires(T &t, AutoParsableRestRequest request) {
  { t(std::move(request)) } -> Convertible;
};
}  // namespace detail

/**
 * @brief Convenient request handler constructible from any callable which may
 * take auto-parsable request and return any convertible value.
 */
class AutoParsableRequestHandler : public IRestRequestHandler {
 public:
  explicit AutoParsableRequestHandler(detail::Callable auto handler)
      : handler_{std::in_place_type_t<Handler>{}, std::move(handler)} {}

  explicit AutoParsableRequestHandler(detail::CallableWithRequest auto handler)
      : handler_{std::in_place_type_t<HandlerWithRequest>{},
                 std::move(handler)} {}

  explicit AutoParsableRequestHandler(detail::CallableWithResponse auto handler)
      : handler_{std::in_place_type_t<HandlerWithResponse>{},
                 [handler = std::move(handler)]() {
                   return ConvertToJson(handler());
                 }} {}

  explicit AutoParsableRequestHandler(
      detail::CallableWithRequestAndResponse auto handler)
      : handler_{
            std::in_place_type_t<HandlerWithRequestAndResponse>{},
            [handler = std::move(handler)](AutoParsableRestRequest request) {
              return ConvertToJson(handler(std::move(request)));
            }} {}

  /**
   * @brief Wraps request in auto-convertible and forwards it to the handler.
   */
  [[nodiscard]] auto HandleRequestAndGiveResponse(RestRequest request) const
      -> RestResponse override;

 private:
  using Handler = fu2::unique_function<void()>;
  using HandlerWithRequest =
      fu2::unique_function<void(AutoParsableRestRequest)>;
  using HandlerWithResponse = fu2::unique_function<Result::value_type()>;
  using HandlerWithRequestAndResponse =
      fu2::unique_function<Result::value_type(AutoParsableRestRequest)>;

  mutable std::variant<Handler, HandlerWithRequest, HandlerWithResponse,
                       HandlerWithRequestAndResponse>
      handler_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_
