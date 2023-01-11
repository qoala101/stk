#include "network_typed_endpoint_sender.h"

#include <coroutine>
#include <exception>
#include <gsl/assert>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "network_exception.h"
#include "network_typed_endpoint.h"
#include "network_types.h"
#include "network_wrong_type_exception.h"

namespace stonks::network {
namespace {
class TypeChecker : public te::EndpointTypesValidatorTemplate {
 public:
  explicit TypeChecker(EndpointTypes endpoint_types)
      : EndpointTypesValidatorTemplate{std::move(endpoint_types)} {}

 private:
  void HandleUnknownParam(std::string_view /*unused*/) const override {
    Expects(false);
  }

  void HandleWrongRequestParamType(
      std::string_view /*unused*/, const Param & /*unused*/,
      const std::exception & /*unused*/) const override {
    Expects(false);
  }

  void HandleMissingRequestBody() const override { Expects(false); }

  void HandleWrongRequestBodyType(
      const Body & /*unused*/,
      const std::exception & /*unused*/) const override {
    Expects(false);
  }

  void HandleUnexpectedRequestBody() const override { Expects(false); }

  void HandleWrongResponseBodyType(
      const Body &response_body,
      const std::exception &parsing_exception) const override {
    throw WrongTypeException{parsing_exception.what(), response_body};
  }

  void HandleUnexpectedResponseBody() const override {
    throw Exception{"Response has unexpected result"};
  }
};
}  // namespace

TypedEndpointSender::TypedEndpointSender(
    EndpointTypes endpoint_types, cpp::NnUp<IRestRequestSender> request_sender)
    : type_checker_{cpp::MakeNnUp<TypeChecker>(std::move(endpoint_types))},
      request_sender_{std::move(request_sender)} {}

auto TypedEndpointSender::SendRequestAndGetResponse(RestRequest request)
    -> cppcoro::task<RestResponse> {
  type_checker_->ValidateRequest(request);
  auto response =
      co_await request_sender_->SendRequestAndGetResponse(std::move(request));
  type_checker_->ValidateResponse(response);
  co_return response;
}
}  // namespace stonks::network