/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_EXCEPTION_H_
#define VH_NETWORK_EXCEPTION_H_

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

#endif  // VH_NETWORK_EXCEPTION_H_
