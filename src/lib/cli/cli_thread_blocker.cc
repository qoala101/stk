#include "cli_thread_blocker.h"

namespace vh::cli {
void ThreadBlocker::BlockThread() {
  auto lock = std::unique_lock<std::mutex>{mutex};
  cond_var.wait(lock);
}

void ThreadBlocker::UnblockThread() { cond_var.notify_one(); }
}  // namespace vh::cli