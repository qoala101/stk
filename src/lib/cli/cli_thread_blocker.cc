/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "cli_thread_blocker.h"

namespace vh::cli {
void ThreadBlocker::BlockThread() {
  auto lock = std::unique_lock<std::mutex>{mutex};
  cond_var.wait(lock);
}

void ThreadBlocker::UnblockThread() { cond_var.notify_one(); }
}  // namespace vh::cli