#ifndef STONKS_APP_APP_WAIT_FOR_INTERRUPT_H_
#define STONKS_APP_APP_WAIT_FOR_INTERRUPT_H_

namespace stonks::app {
/**
 * @brief Blocks current thread until process receives interrupt signal.
 */
void WaitForInterrupt();
}  // namespace stonks::app

#endif  // STONKS_APP_APP_WAIT_FOR_INTERRUPT_H_
