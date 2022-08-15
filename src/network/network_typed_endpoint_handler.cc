#include "network_typed_endpoint_handler.h"

#include <bits/exception.h>
#include <polymorphic_value.h>

#include <functional>
#include <gsl/assert>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include "network_enums.h"
#include "network_json_basic_conversions.h"
#include "network_typed_endpoint.h"
#include "network_types.h"

namespace stonks::network {
TypedEndpointHandler::TypedEndpointHandler(EndpointTypes endpoint_types,
                                           RestRequestHandler handler)
    : endpoint_types_{std::move(endpoint_types)},
      handler_{std::move(handler)} {}

auto TypedEndpointHandler::operator()(RestRequest request) const
    -> RestResponse {
  try {
    ValidateRequest(request);
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

  ValidateResponse(*response);
  return std::move(*response);
}

void TypedEndpointHandler::ValidateRequest(const RestRequest &request) const {
  ValidateRequestParamTypes(request.params);
  ValidateRequestBodyType(request.body);
}

void TypedEndpointHandler::ValidateRequestParamTypes(
    const Params &params) const {
  if (params.size() != endpoint_types_.params.size()) {
    throw std::runtime_error{"Wrong number of request params"};
  }

  for (const auto &[key, value] : params) {
    const auto param_type = endpoint_types_.params.find(key);
    if (param_type == endpoint_types_.params.end()) {
      throw std::runtime_error{"Unknown request param: " + key};
    }

    param_type->second(*value);
  }
}

void TypedEndpointHandler::ValidateRequestBodyType(const Body &body) const {
  if (endpoint_types_.body.has_value()) {
    if (!body.has_value()) {
      throw std::runtime_error{"Request body is missing"};
    }

    (*endpoint_types_.body)(**body);
    return;
  }

  if (body.has_value()) {
    throw std::runtime_error{"Request has unexpected body"};
  }
}

void TypedEndpointHandler::ValidateResponse(
    const RestResponse &response) const {
  if (endpoint_types_.result.has_value()) {
    Expects(response.result.has_value());

    try {
      (*endpoint_types_.result)(**response.result);
    } catch (const std::exception &) {
      Expects(false);
    }

    return;
  }

  Expects(!response.result.has_value());
}
}  // namespace stonks::network