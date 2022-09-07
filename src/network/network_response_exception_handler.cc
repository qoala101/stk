#include "network_response_exception_handler.h"

#include <bits/exception.h>

#include <memory>
#include <optional>
#include <utility>

#include "cpp_message_exception.h"
#include "cpp_optional.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"
#include "network_json_common_conversions.h"
#include "network_types.h"
#include "network_wrong_type_exception.h"

namespace stonks::network {
namespace {
[[nodiscard]] auto TryToParseException(const IJson &json)
    -> cpp::Opt<cpp::MessageException> {
  try {
    return ParseFromJson<cpp::MessageException>(json);
  } catch (const std::exception &) {
    return std::nullopt;
  }
}
}  // namespace

ResponseExceptionHandler::ResponseExceptionHandler(
    cpp::NnSp<IRestRequestSender> request_sender)
    : request_sender_{std::move(request_sender)} {}

auto ResponseExceptionHandler::SendRequestAndGetResponse(
    RestRequest request) const -> RestResponse {
  try {
    return request_sender_->SendRequestAndGetResponse(std::move(request));
  } catch (const WrongTypeException &e) {
    auto server_exception = TryToParseException(*e.GetJson());

    if (const auto server_send_exception = server_exception.has_value()) {
      throw cpp::MessageException{std::move(*server_exception)};
    }

    throw;
  }
}
}  // namespace stonks::network