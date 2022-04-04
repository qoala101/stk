#include "finance_enum_conversions.h"

#include <chrono>

#include "utils.h"

namespace {
template <typename T>
int64_t ConvertToMillis(T time) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
}
}  // namespace

namespace stonks::finance {
int64_t ConvertIntervalToMillis(Interval interval) {
  switch (interval) {
    case Interval::k1Minute:
      return ConvertToMillis(std::chrono::minutes(1));
    case Interval::k3Minutes:
      return ConvertToMillis(std::chrono::minutes(3));
    case Interval::k5Minutes:
      return ConvertToMillis(std::chrono::minutes(5));
    case Interval::k15Minutes:
      return ConvertToMillis(std::chrono::minutes(15));
    case Interval::k30Minutes:
      return ConvertToMillis(std::chrono::minutes(30));
    case Interval::k1Hour:
      return ConvertToMillis(std::chrono::hours(1));
    case Interval::k2Hours:
      return ConvertToMillis(std::chrono::hours(2));
    case Interval::k4Hours:
      return ConvertToMillis(std::chrono::hours(4));
    case Interval::k6Hours:
      return ConvertToMillis(std::chrono::hours(6));
    case Interval::k8Hours:
      return ConvertToMillis(std::chrono::hours(8));
    case Interval::k12Hours:
      return ConvertToMillis(std::chrono::hours(12));
    case Interval::k1Day:
      return ConvertToMillis(std::chrono::days(1));
    case Interval::k3Days:
      return ConvertToMillis(std::chrono::days(3));
    case Interval::k1Week:
      return ConvertToMillis(std::chrono::weeks(1));
    case Interval::k1Month:
      return ConvertToMillis(std::chrono::months(1));
    default:
      break;
  }

  return {};
}
}  // namespace stonks::finance