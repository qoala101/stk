/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "cpp_message_exception.h"

#include <utility>

namespace vh::cpp {
MessageException::MessageException(std::string message)
    : message_{std::move(message)} {}

auto MessageException::what() const noexcept -> const char* {
  return message_.c_str();
}
}  // namespace vh::cpp