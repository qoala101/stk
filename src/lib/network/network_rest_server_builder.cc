/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "network_rest_server_builder.h"

#include <gsl/assert>
#include <memory>
#include <not_null.hpp>

#include "cpp_not_null.h"
#include "network_endpoint_request_dispatcher.h"
#include "network_request_exception_handler.h"
#include "network_typed_endpoint.h"
#include "network_typed_endpoint_handler.h"
#include "network_types.h"

namespace vh::network {
RestServerBuilder::RestServerBuilder(
    Uri base_uri, cpp::NnUp<IRestRequestReceiver> request_receiver)
    : base_uri_{std::move(base_uri)},
      request_receiver_{std::move(request_receiver).as_nullable()} {
  Ensures(base_uri_.has_value());
  Ensures(request_receiver_ != nullptr);
}

auto RestServerBuilder::Start() -> RestServer {
  Expects(request_receiver_ != nullptr);
  Expects(base_uri_.has_value());
  Expects(!endpoint_handlers_.empty());

  request_receiver_->Receive(
      std::move(*base_uri_),
      cpp::MakeNnUp<EndpointRequestDispatcher>(std::move(endpoint_handlers_)));

  auto server = RestServer{cpp::AssumeNn(std::move(request_receiver_))};

  base_uri_.reset();
  endpoint_handlers_.clear();

  Ensures(request_receiver_ == nullptr);
  Ensures(!base_uri_.has_value());
  Ensures(endpoint_handlers_.empty());
  return server;
}

auto RestServerBuilder::Handling(TypedEndpoint endpoint,
                                 cpp::NnUp<IRestRequestHandler> handler)
    -> RestServerBuilder& {
  auto decorated_handler = cpp::MakeNnUp<RequestExceptionHandler>(
      cpp::MakeNnUp<TypedEndpointHandler>(std::move(endpoint.expected_types),
                                          std::move(handler)));

  endpoint_handlers_.emplace(std::move(endpoint.endpoint),
                             std::move(decorated_handler));

  Ensures(!endpoint_handlers_.empty());
  return *this;
}
}  // namespace vh::network