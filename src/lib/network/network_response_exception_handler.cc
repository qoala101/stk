/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "network_response_exception_handler.h"

#include <coroutine>
#include <exception>
#include <not_null.hpp>
#include <utility>

#include "cpp_message_exception.h"
#include "cpp_optional.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"
#include "network_json_conversions_facades.h"
#include "network_types.h"
#include "network_wrong_type_exception.h"

namespace vh::network {
namespace {
auto TryToParseException [[nodiscard]] (const IJson &json)
-> cpp::Opt<cpp::MessageException> {
  try {
    return ParseFromJson<cpp::MessageException>(json);
  } catch (const std::exception &) {
    return {};
  }
}
}  // namespace

ResponseExceptionHandler::ResponseExceptionHandler(
    cpp::NnSp<IRestRequestSender> request_sender)
    : request_sender_{std::move(request_sender)} {}

auto ResponseExceptionHandler::SendRequestAndGetResponse(RestRequest request)
    -> cppcoro::task<RestResponse> {
  try {
    co_return co_await request_sender_->SendRequestAndGetResponse(
        std::move(request));
  } catch (const WrongTypeException &e) {
    auto server_exception = TryToParseException(*e.GetJson());

    if (const auto server_send_exception = server_exception.has_value()) {
      throw cpp::MessageException{std::move(*server_exception)};
    }

    throw;
  }
}
}  // namespace vh::network