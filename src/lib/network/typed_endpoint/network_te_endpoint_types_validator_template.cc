#include "network_te_endpoint_types_validator_template.h"

#include <polymorphic_value.h>

#include <function2/function2.hpp>
#include <gsl/assert>
#include <map>
#include <range/v3/range/conversion.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <utility>

#include "network_json_base_conversions.h"

namespace vh::network::te {
EndpointTypesValidatorTemplate::EndpointTypesValidatorTemplate(
    EndpointTypes endpoint_types)
    : endpoint_types_{std::move(endpoint_types)} {}

void EndpointTypesValidatorTemplate::ValidateRequest(
    const RestRequest &request) const {
  ValidateRequestParamTypes(request.params);
  ValidateRequestBodyType(request.body);
}

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
  const auto null_params =
      endpoint_types_.params |
      ranges::views::transform(
          [](const auto &endpoint_param) { return endpoint_param.first; }) |
      ranges::views::filter([&params](const auto &endpoint_param_name) {
        return !params.contains(endpoint_param_name);
      }) |
      ranges::views::transform([](const auto &endpoint_param_name) {
        return std::pair{endpoint_param_name, CreateNullJson()};
      }) |
      ranges::to<Params>;

  for (const auto &param_map : {&params, &null_params}) {
    for (const auto &[key, value] : *param_map) {
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
}  // namespace vh::network::te