/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CPP_MESSAGE_EXCEPTION_H_
#define VH_CPP_MESSAGE_EXCEPTION_H_

#include <exception>
#include <string>

namespace vh::cpp {
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
}  // namespace vh::cpp

#endif  // VH_CPP_MESSAGE_EXCEPTION_H_
