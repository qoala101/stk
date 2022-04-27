#ifndef STONKS_PRICES_STREAM_H_
#define STONKS_PRICES_STREAM_H_

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include "finance_types.h"

namespace stonks::finance {
class PricesStream {
 public:
  explicit PricesStream(Symbol symbol, std::chrono::milliseconds interval =
                                           std::chrono::milliseconds{1000});
  ~PricesStream();

  void Start();
  void Stop();

  std::vector<TimeDouble> GetPrices(int drop_first = 0);

 private:
  const Symbol symbol_{};
  const std::chrono::milliseconds interval_{};

  std::vector<TimeDouble> prices_{};
  std::mutex prices_mutex_{};
  std::condition_variable prices_cond_var_{};

  std::thread thread_{};
  std::atomic_bool keep_alive_{};
};
}  // namespace stonks::finance

#endif  // STONKS_PRICES_STREAM_H_
