#ifndef STONKS_FINANCE_ENUM_CONVERSIONS_H_
#define STONKS_FINANCE_ENUM_CONVERSIONS_H_

#include <chrono>

#include "finance_enums.h"

namespace stonks::finance {
std::chrono::milliseconds ConvertIntervalToMilliseconds(Interval interval);
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_ENUM_CONVERSIONS_H_
