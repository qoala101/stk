/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CLI_WAIT_FOR_INTERRUPT_H_
#define VH_CLI_WAIT_FOR_INTERRUPT_H_

namespace vh::cli {
/**
 * @brief Blocks current thread until process receives interrupt signal.
 */
void WaitForInterrupt();
}  // namespace vh::cli

#endif  // VH_CLI_WAIT_FOR_INTERRUPT_H_
