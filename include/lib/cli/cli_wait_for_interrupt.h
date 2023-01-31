#ifndef STONKS_CLI_CLI_WAIT_FOR_INTERRUPT_H_
#define STONKS_CLI_CLI_WAIT_FOR_INTERRUPT_H_

namespace stonks::cli {
/**
 * @brief Blocks current thread until process receives interrupt signal.
 */
void WaitForInterrupt();
}  // namespace stonks::cli

#endif  // STONKS_CLI_CLI_WAIT_FOR_INTERRUPT_H_
