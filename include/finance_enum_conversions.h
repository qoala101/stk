#ifndef STONKS_FINANCE_ENUM_CONVERSIONS_H_
#define STONKS_FINANCE_ENUM_CONVERSIONS_H_

#include <cstdint>

#include "finance_enums.h"

namespace stonks::finance {
int64_t ConvertIntervalToMillis(Interval interval);
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_ENUM_CONVERSIONS_H_
