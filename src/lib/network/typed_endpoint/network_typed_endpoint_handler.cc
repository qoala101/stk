/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "network_typed_endpoint_handler.h"

#include <fmt/core.h>

#include <coroutine>
#include <exception>
#include <gsl/assert>
#include <memory>
#include <not_null.hpp>
#include <string>
#include <string_view>
#include <utility>

#include "network_exception.h"
#include "network_te_endpoint_types_validator_template.h"
#include "network_typed_endpoint.h"
#include "network_types.h"
#include "network_wrong_type_exception.h"

namespace vh::network {
namespace {
class TypeChecker : public te::EndpointTypesValidatorTemplate {
 public:
  explicit TypeChecker(EndpointTypes endpoint_types)
      : EndpointTypesValidatorTemplate{std::move(endpoint_types)} {}

 private:
  void HandleUnknownParam(std::string_view param_name) const override {
    throw Exception{fmt::format("Unknown request param: {}", param_name)};
  }

  void HandleWrongRequestParamType(
      std::string_view param_name, const Param &value,
      const std::exception &parsing_exception) const override {
    throw WrongTypeException{
        fmt::format("Request param {} is invalid: {}", param_name.data(),
                    parsing_exception.what()),
        value};
  }

  void HandleMissingRequestBody() const override {
    throw Exception{"Request body is missing"};
  }

  void HandleWrongRequestBodyType(
      const Body &request_body,
      const std::exception &parsing_exception) const override {
    throw WrongTypeException{
        fmt::format("Request body is invalid: {}", parsing_exception.what()),
        request_body};
  }

  void HandleUnexpectedRequestBody() const override {
    throw Exception{"Request has unexpected body"};
  }

  void HandleWrongResponseBodyType(
      const Body & /*unused*/,
      const std::exception & /*unused*/) const override {
    Expects(false);
  }

  void HandleUnexpectedResponseBody() const override { Expects(false); }
};
}  // namespace

TypedEndpointHandler::TypedEndpointHandler(
    EndpointTypes endpoint_types, cpp::NnUp<IRestRequestHandler> handler)
    : type_checker_{cpp::MakeNnUp<TypeChecker>(std::move(endpoint_types))},
      handler_{std::move(handler)} {}

auto TypedEndpointHandler::HandleRequestAndGiveResponse(RestRequest request)
    -> cppcoro::task<RestResponse> {
  type_checker_->ValidateRequest(request);
  auto response =
      co_await handler_->HandleRequestAndGiveResponse(std::move(request));
  type_checker_->ValidateResponse(response);
  co_return response;
}
}  // namespace vh::network