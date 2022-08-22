#include "network_typed_endpoint_sender.h"

#include <bits/exception.h>

#include <gsl/assert>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "network_typed_endpoint.h"
#include "network_types.h"

namespace stonks::network {
namespace {
class TypeChecker : public EndpointTypesValidatorTemplate {
  using EndpointTypesValidatorTemplate::EndpointTypesValidatorTemplate;

  void HandleWrongParamsSize() const override { Expects(false); }

  void HandleUnknownParam(std::string_view /*param_name*/) const override {
    Expects(false);
  }

  void HandleWrongRequestParamType(
      const std::exception & /*exception*/) const override {
    Expects(false);
  }

  void HandleMissingRequestBody() const override { Expects(false); }

  void HandleWrongRequestBodyType(
      const std::exception & /*exception*/) const override {
    Expects(false);
  }

  void HandleUnexpectedRequestBody() const override { Expects(false); }

  void HandleMissingResponseBody() const override {
    throw std::runtime_error{"Response is missing result"};
  }

  void HandleWrongResponseBodyType(
      const std::exception &exception) const override {
    throw exception;
  }

  void HandleUnexpectedResponseBody() const override {
    throw std::runtime_error{"Response has unexpected result"};
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