#ifndef STONKS_FINANCE_API_H_
#define STONKS_FINANCE_API_H_

#include <optional>
#include <string_view>
#include <vector>

#include "finance_types.h"

namespace stonks {
std::optional<std::vector<Candlestick>> GetCandlesticks(
    std::string_view symbol);
}

#endif  // STONKS_FINANCE_API_H_
