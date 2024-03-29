/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_RESTSDK_CALL_AS_COROUTINE_H_
#define VH_RESTSDK_CALL_AS_COROUTINE_H_

#include <cppcoro/single_consumer_event.hpp>
#include <cppcoro/task.hpp>
#include <gsl/assert>

#include "cpp_message_exception.h"

namespace pplx {
template <typename T>
class task;
}  // namespace pplx

namespace vh::restsdk {
/**
 * @brief Util which allows to call tasks as coroutines.
 */
template <typename T>
auto CallAsCoroutine [[nodiscard]] (const pplx::task<T> &task)
-> cppcoro::task<> {
  auto exception_message = std::string{};
  auto task_is_completed = cppcoro::single_consumer_event{};

  task.then([&exception_message, &task_is_completed](const auto &task) {
    try {
      task.wait();
    } catch (const std::exception &e) {
      exception_message = e.what();
    }

    task_is_completed.set();
  });

  co_await task_is_completed;

  if (!exception_message.empty()) {
    throw cpp::MessageException{std::move(exception_message)};
  }
}
}  // namespace vh::restsdk

#endif  // VH_RESTSDK_CALL_AS_COROUTINE_H_
