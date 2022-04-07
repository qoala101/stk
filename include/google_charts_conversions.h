#ifndef STONKS_GOOGLE_CHARTS_CONVERSIONS_H_
#define STONKS_GOOGLE_CHARTS_CONVERSIONS_H_

#include <cpprest/json.h>

#include "finance_types.h"

namespace stonks {
web::json::value ConvertToJson(const std::vector<finance::Candlestick> &data);

web::json::value ConvertToJson(
    const std::vector<finance::StrategyOrderRequest> &data);
}  // namespace stonks

#endif  // STONKS_GOOGLE_CHARTS_CONVERSIONS_H_
