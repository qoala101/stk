#ifndef STONKS_GOOGLE_CHARTS_CONVERSIONS_H_
#define STONKS_GOOGLE_CHARTS_CONVERSIONS_H_

#include <cpprest/json.h>

#include "finance_types.h"

namespace stonks {
web::json::value ConvertToJson(const std::vector<finance::Candlestick> &data);
}

#endif  // STONKS_GOOGLE_CHARTS_CONVERSIONS_H_
