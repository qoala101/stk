#ifndef VH_NETWORK_TYPED_ENDPOINT_HANDLER_H_
#define VH_NETWORK_TYPED_ENDPOINT_HANDLER_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "network_i_rest_request_handler.h"
#include "network_te_endpoint_types_validator_template.h"
#include "network_typed_endpoint.h"
#include "network_types.h"

namespace vh::network {
/**
 * @brief Decorator of request handler which adds validation of request
 * and response types.
 */
class TypedEndpointHandler : public IRestRequestHandler {
 public:
  /**
   * @param endpoint_types Request and response would be validated against
   * those.
   */
  TypedEndpointHandler(EndpointTypes endpoint_types,
                       cpp::NnUp<IRestRequestHandler> handler);

  /**
   * @brief Validates request prior forwarding it to the handler and response
   * after receiving it from handler.
   */
  auto HandleRequestAndGiveResponse [[nodiscard]] (RestRequest request)
  -> cppcoro::task<RestResponse> override;

 private:
  cpp::NnUp<te::EndpointTypesValidatorTemplate> type_checker_;
  cpp::NnUp<IRestRequestHandler> handler_;
};
}  // namespace vh::network

#endif  // VH_NETWORK_TYPED_ENDPOINT_HANDLER_H_
