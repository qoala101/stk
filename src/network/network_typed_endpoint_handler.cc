#include "network_typed_endpoint_handler.h"

#include <bits/exception.h>
#include <polymorphic_value.h>

#include <gsl/assert>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "network_endpoint_types_validator_template.h"
#include "network_enums.h"
#include "network_json_basic_conversions.h"
#include "network_typed_endpoint.h"
#include "network_types.h"
#include "not_null.hpp"

namespace stonks::network {
namespace {
class TypeChecker : public EndpointTypesValidatorTemplate {
  using EndpointTypesValidatorTemplate::EndpointTypesValidatorTemplate;

  void HandleWrongParamsSize() const override {
    throw std::runtime_error{"Wrong number of request params"};
  }

  void HandleUnknownParam(std::string_view param_name) const override {
    throw std::runtime_error{std::string{"Unknown request param: "} +
                             param_name.data()};
  }

  void HandleWrongRequestParamType(
      const std::exception &exception) const override {
    throw exception;
  }

  void HandleMissingRequestBody() const override {
    throw std::runtime_error{"Request body is missing"};
  }

  void HandleWrongRequestBodyType(
      const std::exception &exception) const override {
    throw exception;
  }

  void HandleUnexpectedRequestBody() const override {
    throw std::runtime_error{"Request has unexpected body"};
  }

  void HandleMissingResponseBody() const override { Expects(false); }

  void HandleWrongResponseBodyType(
      const std::exception &exception) const override {
    Expects(false);
  }

  void HandleUnexpectedResponseBody() const override { Expects(false); }
};
}  // namespace

TypedEndpointHandler::TypedEndpointHandler(EndpointTypes endpoint_types,
                                           RestRequestHandler handler)
    : type_checker_{cpp::assume_not_null(
          std::make_shared<TypeChecker>(std::move(endpoint_types)))},
      handler_{std::move(handler)} {}

auto TypedEndpointHandler::operator()(RestRequest request) const
    -> RestResponse {
  try {
    type_checker_->ValidateRequest(request);
  } catch (const std::exception &exception) {
    return {.status = Status::kBadRequest,
            .result = ConvertToJson(std::runtime_error{"Wrong request"})};
  }

  auto response = std::optional<RestResponse>{};

  try {
    response = handler_(std::move(request));
  } catch (const std::exception &exception) {
    return {.status = Status::kInternalError,
            .result = ConvertToJson(std::runtime_error{exception.what()})};
  }

  type_checker_->ValidateResponse(*response);
  return std::move(*response);
}
}  // namespace stonks::network