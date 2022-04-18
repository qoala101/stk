#ifndef STONKS_GOOGLE_CHARTS_CONVERSIONS_H_
#define STONKS_GOOGLE_CHARTS_CONVERSIONS_H_

#include <cpprest/json.h>

#include "finance_types.h"

namespace stonks {
web::json::value ConvertToJson(const std::vector<finance::Candle> &data);

web::json::value ConvertToJson(
    const std::vector<finance::StrategyOrderRequest> &data);

web::json::value ConvertToJson(const std::vector<finance::TimeDouble> &data);
}  // namespace stonks

#endif  // STONKS_GOOGLE_CHARTS_CONVERSIONS_H_
