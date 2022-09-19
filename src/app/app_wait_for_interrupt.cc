#include "app_wait_for_interrupt.h"

#include <condition_variable>
#include <csignal>
#include <mutex>
#include <tuple>

namespace stonks::app {
namespace {
class ThreadBlocker {
 public:
  void BlockCurrentThread() {
    auto lock = std::unique_lock<std::mutex>{mutex};
    cond_var.wait(lock);
  }

  void Unblock() { cond_var.notify_one(); }

 private:
  std::mutex mutex{};
  std::condition_variable cond_var{};
};
}  // namespace

void WaitForInterrupt() {
  static auto thread_blocker = ThreadBlocker{};
  static auto unblock = [](int) { thread_blocker.Unblock(); };
  std::ignore = std::signal(SIGINT, unblock);
  std::ignore = std::signal(SIGTERM, unblock);
  thread_blocker.BlockCurrentThread();
}
}  // namespace stonks::app