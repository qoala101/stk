#ifndef STONKS_GOOGLE_CHARTS_GOOGLE_CHARTS_CONVERSIONS_H_
#define STONKS_GOOGLE_CHARTS_GOOGLE_CHARTS_CONVERSIONS_H_

#include <cpprest/json.h>

#include <vector>

#include "finance_types.h"

namespace stonks::google_charts {
web::json::value ConvertToJson(const std::vector<finance::Candle> &data);

web::json::value ConvertToJson(
    const std::vector<finance::StrategyOrderRequest> &data);

web::json::value ConvertToJson(const std::vector<finance::TimeDouble> &data);

web::json::value ConvertToJson(const std::vector<finance::Order> &data);
}  // namespace stonks::google_charts

#endif  // STONKS_GOOGLE_CHARTS_GOOGLE_CHARTS_CONVERSIONS_H_
