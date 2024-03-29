/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_TE_ENDPOINT_TYPES_VALIDATOR_TEMPLATE_H_
#define VH_NETWORK_TE_ENDPOINT_TYPES_VALIDATOR_TEMPLATE_H_

#include <exception>
#include <string_view>

#include "network_typed_endpoint.h"
#include "network_types.h"

namespace vh::network::te {
/**
 * @brief Template of REST request-response type checking algorithm.
 * Client would extend and handle the detected errors.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class EndpointTypesValidatorTemplate {
 public:
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

 protected:
  /**
   * @param endpoint_types Request and response would be validated against
   * those.
   */
  explicit EndpointTypesValidatorTemplate(EndpointTypes endpoint_types);

 private:
  virtual void HandleUnknownParam(std::string_view param_name) const;
  virtual void HandleWrongRequestParamType(
      std::string_view param_name, const Param &value,
      const std::exception &parsing_exception) const;
  virtual void HandleMissingRequestBody() const;
  virtual void HandleWrongRequestBodyType(
      const Body &request_body, const std::exception &parsing_exception) const;
  virtual void HandleUnexpectedRequestBody() const;
  virtual void HandleWrongResponseBodyType(
      const Body &response_body, const std::exception &parsing_exception) const;
  virtual void HandleUnexpectedResponseBody() const;

  void ValidateRequestParamTypes(const Params &params) const;
  void ValidateRequestBodyType(const Body &body) const;

  EndpointTypes endpoint_types_{};
};
}  // namespace vh::network::te

#endif  // VH_NETWORK_TE_ENDPOINT_TYPES_VALIDATOR_TEMPLATE_H_
