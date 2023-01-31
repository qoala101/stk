#ifndef STONKS_NETWORK_NETWORK_EXCEPTION_H_
#define STONKS_NETWORK_NETWORK_EXCEPTION_H_

#include "cpp_message_exception.h"

namespace vh::network {
/**
 * @brief Generic network exception.
 */
class Exception : public cpp::MessageException {
 public:
  using cpp::MessageException::MessageException;
};
}  // namespace vh::network

#endif  // STONKS_NETWORK_NETWORK_EXCEPTION_H_
