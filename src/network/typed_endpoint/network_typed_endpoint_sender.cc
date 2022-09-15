#include "network_typed_endpoint_sender.h"

#include <bits/exception.h>

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
class TypeChecker : public EndpointTypesValidatorTemplate {
 public:
  explicit TypeChecker(EndpointTypes endpoint_types)
      : EndpointTypesValidatorTemplate{std::move(endpoint_types)} {}

 private:
  void HandleWrongParamsSize() const override { Expects(false); }

  void HandleUnknownParam(std::string_view /*param_name*/) const override {
    Expects(false);
  }

  void HandleWrongRequestParamType(
      std::string_view /*param_name*/, const Param & /*value*/,
      const std::exception & /*parsing_exception*/) const override {
    Expects(false);
  }

  void HandleMissingRequestBody() const override { Expects(false); }

  void HandleWrongRequestBodyType(
      const Body::value_type & /*request_body*/,
      const std::exception & /*parsing_exception*/) const override {
    Expects(false);
  }

  void HandleUnexpectedRequestBody() const override { Expects(false); }

  void HandleMissingResponseBody() const override {
    throw Exception{"Response is missing result"};
  }

  void HandleWrongResponseBodyType(
      const Body::value_type &response_body,
      const std::exception &parsing_exception) const override {
    throw WrongTypeException{parsing_exception.what(), response_body};
  }

  void HandleUnexpectedResponseBody() const override {
    throw Exception{"Response has unexpected result"};
  }
};
}  // namespace

TypedEndpointSender::TypedEndpointSender(
    EndpointTypes endpoint_types, cpp::NnSp<IRestRequestSender> request_sender)
    : type_checker_{cpp::MakeNnUp<TypeChecker>(std::move(endpoint_types))},
      request_sender_{std::move(request_sender)} {}

auto TypedEndpointSender::SendRequestAndGetResponse(RestRequest request) const
    -> RestResponse {
  type_checker_->ValidateRequest(request);
  auto response =
      request_sender_->SendRequestAndGetResponse(std::move(request));
  type_checker_->ValidateResponse(response);
  return response;
}
}  // namespace stonks::network