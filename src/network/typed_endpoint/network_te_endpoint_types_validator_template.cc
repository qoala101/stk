#include "network_te_endpoint_types_validator_template.h"

#include <polymorphic_value.h>

#include <function2/function2.hpp>
#include <map>
#include <string>
#include <type_traits>
#include <utility>

#include "cpp_optional.h"

namespace stonks::network::te {
EndpointTypesValidatorTemplate::EndpointTypesValidatorTemplate(
    EndpointTypes endpoint_types)
    : endpoint_types_{std::move(endpoint_types)} {}

void EndpointTypesValidatorTemplate::ValidateRequest(
    const RestRequest &request) const {
  ValidateRequestParamTypes(request.params);
  ValidateRequestBodyType(request.body);
}

void EndpointTypesValidatorTemplate::HandleWrongParamsSize() const {}

void EndpointTypesValidatorTemplate::HandleUnknownParam(
    std::string_view /*param_name*/) const {}

void EndpointTypesValidatorTemplate::HandleWrongRequestParamType(
    std::string_view /*param_name*/, const Param & /*value*/,
    const std::exception & /*parsing_exception*/) const {}

void EndpointTypesValidatorTemplate::HandleMissingRequestBody() const {}

void EndpointTypesValidatorTemplate::HandleWrongRequestBodyType(
    const Body::value_type & /*request_body*/,
    const std::exception & /*parsing_exception*/) const {}

void EndpointTypesValidatorTemplate::HandleUnexpectedRequestBody() const {}

void EndpointTypesValidatorTemplate::HandleMissingResponseBody() const {}

void EndpointTypesValidatorTemplate::HandleWrongResponseBodyType(
    const Body::value_type & /*response_body*/,
    const std::exception & /*parsing_exception*/) const {}

void EndpointTypesValidatorTemplate::HandleUnexpectedResponseBody() const {}

void EndpointTypesValidatorTemplate::ValidateRequestParamTypes(
    const Params &params) const {
  if (params.size() != endpoint_types_.params.size()) {
    HandleWrongParamsSize();
  }

  for (const auto &[key, value] : params) {
    const auto param_type = endpoint_types_.params.find(key);
    if (param_type == endpoint_types_.params.end()) {
      HandleUnknownParam(key);
    }

    try {
      param_type->second(*value);
    } catch (const std::exception &e) {
      HandleWrongRequestParamType(key, value, e);
    }
  }
}

void EndpointTypesValidatorTemplate::ValidateRequestBodyType(
    const Body &body) const {
  if (endpoint_types_.body.has_value()) {
    if (!body.has_value()) {
      HandleMissingRequestBody();
      return;
    }

    try {
      (*endpoint_types_.body)(**body);
    } catch (const std::exception &e) {
      HandleWrongRequestBodyType(*body, e);
    }

    return;
  }

  if (body.has_value()) {
    HandleUnexpectedRequestBody();
  }
}

void EndpointTypesValidatorTemplate::ValidateResponse(
    const RestResponse &response) const {
  if (endpoint_types_.result.has_value()) {
    if (!response.result.has_value()) {
      HandleMissingResponseBody();
      return;
    }

    try {
      (*endpoint_types_.result)(**response.result);
    } catch (const std::exception &e) {
      HandleWrongResponseBodyType(*response.result, e);
    }

    return;
  }

  if (response.result.has_value()) {
    HandleUnexpectedResponseBody();
  }
}
}  // namespace stonks::network::te