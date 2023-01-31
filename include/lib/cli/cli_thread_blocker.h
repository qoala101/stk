#ifndef VH_CLI_THREAD_BLOCKER_H_
#define VH_CLI_THREAD_BLOCKER_H_

#include <condition_variable>
#include <mutex>

namespace vh::cli {
/**
 * @brief API to block the thread.
 */
class ThreadBlocker {
 public:
  /**
   * @brief Blocks current thread until unblocked.
   */
  void BlockThread();

  /**
   * @brief Unblocks current thread.
   */
  void UnblockThread();

 private:
  std::mutex mutex{};
  std::condition_variable cond_var{};
};
}  // namespace vh::cli

#endif  // VH_CLI_THREAD_BLOCKER_H_
