#include "network_typed_endpoint_sender.h"

#include <gsl/assert>
#include <range/v3/view/set_algorithm.hpp>
#include <range/v3/view/transform.hpp>
#include <stdexcept>
#include <string>
#include <utility>

#include "network_typed_endpoint.h"
#include "network_types.h"

namespace stonks::network {

TypedEndpointSender::TypedEndpointSender(
    EndpointTypes endpoint_types,
    cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender)
    : endpoint_types_{std::move(endpoint_types)},
      request_sender_{std::move(request_sender)} {}

auto TypedEndpointSender::SendRequestAndGetResponse(RestRequest request) const
    -> RestResponse {
  ValidateRequest(request);
  auto response =
      request_sender_->SendRequestAndGetResponse(std::move(request));
  ValidateResponseTypes(response);
  return response;
}

void TypedEndpointSender::ValidateRequest(const RestRequest &request) const {
  ValidateRequestParamTypes(request.params);
  ValidateRequestBodyType(request.body);
}

void TypedEndpointSender::ValidateRequestParamTypes(
    const Params &params) const {
  Expects(params.size() == endpoint_types_.params.size());

  for (const auto &[key, value] : params) {
    const auto param_type = endpoint_types_.params.find(key);
    Expects(param_type != endpoint_types_.params.end());

    try {
      param_type->second(*value);
    } catch (const std::exception &) {
      Expects(false);
    }
  }
}
void TypedEndpointSender::ValidateRequestBodyType(const Body &body) const {
  if (endpoint_types_.body.has_value()) {
    Expects(body.has_value());

    try {
      (*endpoint_types_.body)(**body);
    } catch (const std::exception &) {
      Expects(false);
    }

    return;
  }

  Expects(!body.has_value());
}

void TypedEndpointSender::ValidateResponseTypes(
    const RestResponse &response) const {
  // if (endpoint_types_.result.has_value()) {
  //   if (!response.result.has_value()) {
  //     throw std::runtime_error{"Response doesn't have expected result"};
  //   }

  //   (*endpoint_types_.result)(**response.result);
  // }
}
}  // namespace stonks::network