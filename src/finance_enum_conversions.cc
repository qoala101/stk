#include "finance_enum_conversions.h"

#include <chrono>

#include "utils.h"

namespace {
template <typename T>
std::chrono::milliseconds ConvertToMilliseconds(T time) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(time);
}
}  // namespace

namespace stonks::finance {
std::chrono::milliseconds ConvertIntervalToMilliseconds(Interval interval) {
  switch (interval) {
    case Interval::k1Minute:
      return ConvertToMilliseconds(std::chrono::minutes{1});
    case Interval::k3Minutes:
      return ConvertToMilliseconds(std::chrono::minutes{3});
    case Interval::k5Minutes:
      return ConvertToMilliseconds(std::chrono::minutes{5});
    case Interval::k15Minutes:
      return ConvertToMilliseconds(std::chrono::minutes{15});
    case Interval::k30Minutes:
      return ConvertToMilliseconds(std::chrono::minutes{30});
    case Interval::k1Hour:
      return ConvertToMilliseconds(std::chrono::hours{1});
    case Interval::k2Hours:
      return ConvertToMilliseconds(std::chrono::hours{2});
    case Interval::k4Hours:
      return ConvertToMilliseconds(std::chrono::hours{4});
    case Interval::k6Hours:
      return ConvertToMilliseconds(std::chrono::hours{6});
    case Interval::k8Hours:
      return ConvertToMilliseconds(std::chrono::hours{8});
    case Interval::k12Hours:
      return ConvertToMilliseconds(std::chrono::hours{12});
    case Interval::k1Day:
      return ConvertToMilliseconds(std::chrono::days{1});
    case Interval::k3Days:
      return ConvertToMilliseconds(std::chrono::days{3});
    case Interval::k1Week:
      return ConvertToMilliseconds(std::chrono::weeks{1});
    case Interval::k1Month:
      return ConvertToMilliseconds(std::chrono::months{1});
    default:
      break;
  }

  return {};
}
}  // namespace stonks::finance