#include "cpp_message_exception.h"

#include <utility>

namespace stonks::cpp {
MessageException::MessageException(std::string message)
    : message_{std::move(message)} {}

auto MessageException::what() const noexcept -> const char* {
  return message_.c_str();
}
}  // namespace stonks::cpp