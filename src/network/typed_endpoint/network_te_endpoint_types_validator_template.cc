#include "network_te_endpoint_types_validator_template.h"

#include <polymorphic_value.h>

#include <function2/function2.hpp>
#include <gsl/assert>
#include <map>
#include <string>
#include <utility>

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
    std::string_view /*unused*/) const {}

void EndpointTypesValidatorTemplate::HandleWrongRequestParamType(
    std::string_view /*unused*/, const Param & /*unused*/,
    const std::exception & /*unused*/) const {}

void EndpointTypesValidatorTemplate::HandleMissingRequestBody() const {}

void EndpointTypesValidatorTemplate::HandleWrongRequestBodyType(
    const Body & /*unused*/, const std::exception & /*unused*/) const {}

void EndpointTypesValidatorTemplate::HandleUnexpectedRequestBody() const {}

void EndpointTypesValidatorTemplate::HandleWrongResponseBodyType(
    const Body & /*unused*/, const std::exception & /*unused*/) const {}

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
      Expects(!param_type->second.empty());
      param_type->second(*value);
    } catch (const std::exception &e) {
      HandleWrongRequestParamType(key, value, e);
    }
  }
}

void EndpointTypesValidatorTemplate::ValidateRequestBodyType(
    const Body &body) const {
  if (!endpoint_types_.body.empty()) {
    try {
      endpoint_types_.body(*body);
    } catch (const std::exception &e) {
      HandleWrongRequestBodyType(body, e);
    }

    return;
  }

  if (!body->IsNull()) {
    HandleUnexpectedRequestBody();
  }
}

void EndpointTypesValidatorTemplate::ValidateResponse(
    const RestResponse &response) const {
  if (!endpoint_types_.result.empty()) {
    try {
      endpoint_types_.result(*response.result);
    } catch (const std::exception &e) {
      HandleWrongResponseBodyType(response.result, e);
    }

    return;
  }

  if (!response.result->IsNull()) {
    HandleUnexpectedResponseBody();
  }
}
}  // namespace stonks::network::te