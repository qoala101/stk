#ifndef STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_EXCEPTION_H_
#define STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_EXCEPTION_H_

#include <string>

#include "cpp_message_exception.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"

namespace stonks::network {
/**
 * @brief Generic network exception.
 */
class Exception : public cpp::MessageException {
 public:
  using cpp::MessageException::MessageException;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_EXCEPTION_H_
