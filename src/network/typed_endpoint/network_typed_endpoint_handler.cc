#include "network_typed_endpoint_handler.h"

#include <bits/exception.h>

#include <gsl/assert>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "network_endpoint_types_validator_template.h"
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
  void HandleWrongParamsSize() const override {
    throw Exception{"Wrong number of request params"};
  }

  void HandleUnknownParam(std::string_view param_name) const override {
    throw Exception{std::string{"Unknown request param: "} + param_name.data()};
  }

  void HandleWrongRequestParamType(
      std::string_view param_name, const Param &value,
      const std::exception &parsing_exception) const override {
    throw WrongTypeException{std::string{"Request param "} + param_name.data() +
                                 " is invalid: " + parsing_exception.what(),
                             value};
  }

  void HandleMissingRequestBody() const override {
    throw Exception{"Request body is missing"};
  }

  void HandleWrongRequestBodyType(
      const Body::value_type &request_body,
      const std::exception &parsing_exception) const override {
    throw WrongTypeException{
        std::string{"Request body is invalid: "} + parsing_exception.what(),
        request_body};
  }

  void HandleUnexpectedRequestBody() const override {
    throw Exception{"Request has unexpected body"};
  }

  void HandleMissingResponseBody() const override { Expects(false); }

  void HandleWrongResponseBodyType(
      const Body::value_type & /*response_body*/,
      const std::exception & /*parsing_exception*/) const override {
    Expects(false);
  }

  void HandleUnexpectedResponseBody() const override { Expects(false); }
};
}  // namespace

TypedEndpointHandler::TypedEndpointHandler(
    EndpointTypes endpoint_types, cpp::NnUp<IRestRequestHandler> handler)
    : type_checker_{cpp::MakeNnUp<TypeChecker>(std::move(endpoint_types))},
      handler_{std::move(handler)} {}

auto TypedEndpointHandler::HandleRequestAndGiveResponse(
    RestRequest request) const -> RestResponse {
  type_checker_->ValidateRequest(request);
  auto response = handler_->HandleRequestAndGiveResponse(std::move(request));
  type_checker_->ValidateResponse(response);
  return response;
}
}  // namespace stonks::network