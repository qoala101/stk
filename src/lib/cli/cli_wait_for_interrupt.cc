#include "cli_wait_for_interrupt.h"

#include <csignal>
#include <tuple>

#include "cli_thread_blocker.h"

namespace vh::cli {
void WaitForInterrupt() {
  static auto thread_blocker = ThreadBlocker{};
  static auto unblock_thread = [](auto) { thread_blocker.UnblockThread(); };
  std::ignore = std::signal(SIGINT, unblock_thread);
  std::ignore = std::signal(SIGTERM, unblock_thread);
  thread_blocker.BlockThread();
}
}  // namespace vh::cli