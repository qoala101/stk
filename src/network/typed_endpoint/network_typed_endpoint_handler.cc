#include "network_typed_endpoint_handler.h"

#include <bits/exception.h>

#include <gsl/assert>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "ccutils_polymorphic_value.h"
#include "network_endpoint_types_validator_template.h"
#include "network_enums.h"
#include "network_json_basic_conversions.h"
#include "network_typed_endpoint.h"
#include "network_types.h"

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
      const std::exception & /*exception*/) const override {
    Expects(false);
  }

  void HandleUnexpectedResponseBody() const override { Expects(false); }
};
}  // namespace

TypedEndpointHandler::TypedEndpointHandler(
    EndpointTypes endpoint_types, ccutils::NnUp<IRestRequestHandler> handler)
    : type_checker_{ccutils::MakeNnUp<TypeChecker>(std::move(endpoint_types))},
      handler_{std::move(handler)} {}

auto TypedEndpointHandler::HandleRequestAndGiveResponse(
    RestRequest request) const -> RestResponse {
  try {
    type_checker_->ValidateRequest(request);
  } catch (const std::exception &exception) {
    return {.status = Status::kBadRequest,
            .result = ConvertToJson(std::runtime_error{"Wrong request"})};
  }

  auto response = std::optional<RestResponse>{};

  try {
    response = handler_->HandleRequestAndGiveResponse(std::move(request));
  } catch (const std::exception &exception) {
    return {.status = Status::kInternalError,
            .result = ConvertToJson(std::runtime_error{exception.what()})};
  }

  type_checker_->ValidateResponse(*response);
  return std::move(*response);
}
}  // namespace stonks::network