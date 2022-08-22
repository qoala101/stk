#ifndef STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_
#define STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_

#include <functional>
#include <utility>
#include <variant>

#include "network_auto_parsable_request.h"
#include "network_i_rest_request_handler.h"
#include "network_json_basic_conversions.h"
#include "network_types.h"

namespace stonks::network {
namespace auto_parsable_request_handler {
template <typename T>
concept Callable = requires(const T &t) {
  { t() } -> std::same_as<void>;
};

template <typename T>
concept CallableWithRequest = requires(const T &t,
                                       AutoParsableRestRequest request) {
  { t(std::move(request)) } -> std::same_as<void>;
};

template <typename T>
concept CallableWithResponse = requires(const T &t) {
  { t() } -> Convertible;
};

template <typename T>
concept CallableWithRequestAndResponse =
    requires(const T &t, AutoParsableRestRequest request) {
  { t(std::move(request)) } -> Convertible;
};
}  // namespace auto_parsable_request_handler

/**
 * @brief Convenient request handler constructible from any callable which may
 * take auto-parsable request and return any convertible value.
 */
class AutoParsableRequestHandler : public IRestRequestHandler {
 public:
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  AutoParsableRequestHandler(
      auto_parsable_request_handler::Callable auto handler)
      : handler_{std::in_place_type_t<Handler>{}, std::move(handler)} {}

  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  AutoParsableRequestHandler(
      auto_parsable_request_handler::CallableWithRequest auto handler)
      : handler_{std::in_place_type_t<HandlerWithRequest>{},
                 std::move(handler)} {}

  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  AutoParsableRequestHandler(
      auto_parsable_request_handler::CallableWithResponse auto handler)
      : handler_{std::in_place_type_t<HandlerWithResponse>{},
                 [handler = std::move(handler)]() {
                   return ConvertToJson(handler());
                 }} {}

  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  AutoParsableRequestHandler(
      auto_parsable_request_handler::CallableWithRequestAndResponse auto
          handler)
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
  using Handler = std::function<void()>;
  using HandlerWithRequest = std::function<void(AutoParsableRestRequest)>;
  using HandlerWithResponse = std::function<Result::value_type()>;
  using HandlerWithRequestAndResponse =
      std::function<Result::value_type(AutoParsableRestRequest)>;

  std::variant<Handler, HandlerWithRequest, HandlerWithResponse,
               HandlerWithRequestAndResponse>
      handler_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_HANDLER_H_
