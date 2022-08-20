#ifndef STONKS_NETWORK_TYPE_CHECKING_NETWORK_REST_TYPES_CHECK_TEMPLATE_H_
#define STONKS_NETWORK_TYPE_CHECKING_NETWORK_REST_TYPES_CHECK_TEMPLATE_H_

#include <bits/exception.h>

#include <string_view>

#include "network_typed_endpoint.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Template of REST request-response type checking algorithm.
 * Client would extend and handle the detected errors.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class EndpointTypesValidatorTemplate {
 public:
  /**
   * @param endpoint_types Request and response would be validated against
   * those.
   */
  explicit EndpointTypesValidatorTemplate(EndpointTypes endpoint_types);

  virtual ~EndpointTypesValidatorTemplate() = default;

  /**
   * @brief Checks request for validity and calls Handle methods
   * when errors detected.
   */
  void ValidateRequest(const RestRequest &request) const;

  /**
   * @brief Checks response for validity and calls Handle methods
   * when errors detected.
   */
  void ValidateResponse(const RestResponse &response) const;

 private:
  virtual void HandleWrongParamsSize() const;
  virtual void HandleUnknownParam(std::string_view param_name) const;
  virtual void HandleWrongRequestParamType(
      const std::exception &exception) const;
  virtual void HandleMissingRequestBody() const;
  virtual void HandleWrongRequestBodyType(
      const std::exception &exception) const;
  virtual void HandleUnexpectedRequestBody() const;
  virtual void HandleMissingResponseBody() const;
  virtual void HandleWrongResponseBodyType(
      const std::exception &exception) const;
  virtual void HandleUnexpectedResponseBody() const;

  void ValidateRequestParamTypes(const Params &params) const;
  void ValidateRequestBodyType(const Body &body) const;

  EndpointTypes endpoint_types_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_TYPE_CHECKING_NETWORK_REST_TYPES_CHECK_TEMPLATE_H_
