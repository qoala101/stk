#include "network_endpoint_types_validator_template.h"

#include <polymorphic_value.h>

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

namespace stonks::network {
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
    std::string_view param_name) const {}

void EndpointTypesValidatorTemplate::HandleWrongRequestParamType(
    const std::exception &exception) const {}

void EndpointTypesValidatorTemplate::HandleMissingRequestBody() const {}

void EndpointTypesValidatorTemplate::HandleWrongRequestBodyType(
    const std::exception &exception) const {}

void EndpointTypesValidatorTemplate::HandleUnexpectedRequestBody() const {}

void EndpointTypesValidatorTemplate::HandleMissingResponseBody() const {}

void EndpointTypesValidatorTemplate::HandleWrongResponseBodyType(
    const std::exception &exception) const {}

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
    } catch (const std::exception &exception) {
      HandleWrongRequestParamType(exception);
    }
  }
}

void EndpointTypesValidatorTemplate::ValidateRequestBodyType(
    const Body &body) const {
  if (endpoint_types_.body.has_value()) {
    if (!body.has_value()) {
      HandleMissingRequestBody();
    }

    try {
      (*endpoint_types_.body)(**body);
    } catch (const std::exception &exception) {
      HandleWrongRequestBodyType(exception);
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
    }

    try {
      (*endpoint_types_.result)(**response.result);
    } catch (const std::exception &exception) {
      HandleWrongResponseBodyType(exception);
    }

    return;
  }

  if (response.result.has_value()) {
    HandleUnexpectedResponseBody();
  }
}
}  // namespace stonks::network