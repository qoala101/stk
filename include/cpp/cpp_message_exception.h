#ifndef STONKS_CPP_CPP_MESSAGE_EXCEPTION_H_
#define STONKS_CPP_CPP_MESSAGE_EXCEPTION_H_

#include <bits/exception.h>

#include <string>

namespace stonks::cpp {
/**
 * @brief Exception with custom message.
 */
class MessageException : public std::exception {
 public:
  explicit MessageException(std::string message);

  /**
   * @brief Gives the message.
   */
  auto what [[nodiscard]] () const noexcept -> const char* override;

 private:
  std::string message_{};
};
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_MESSAGE_EXCEPTION_H_
