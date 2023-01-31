#ifndef VH_CLI_WAIT_FOR_INTERRUPT_H_
#define VH_CLI_WAIT_FOR_INTERRUPT_H_

namespace vh::cli {
/**
 * @brief Blocks current thread until process receives interrupt signal.
 */
void WaitForInterrupt();
}  // namespace vh::cli

#endif  // VH_CLI_WAIT_FOR_INTERRUPT_H_
