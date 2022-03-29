#include "json_conversions.h"

#include <spdlog/spdlog.h>

#include <magic_enum.hpp>

#include "enum_conversions.h"

namespace {
const std::string& GetStringPropertyAsString(
    const web::json::value& json, const std::string_view property_name) {
  return json.at(std::string{property_name}).as_string();
}

double GetStringPropertyAsDouble(const web::json::value& json,
                                 const std::string_view property_name) {
  return std::stod(json.at(std::string{property_name}).as_string());
}

int64_t GetInt64PropertyAsInt64(const web::json::value& json,
                                const std::string_view property_name) {
  return json.at(std::string{property_name}).as_number().to_int64();
}

template <typename E>
E GetStringPropertyAsEnum(const web::json::value& json,
                          const std::string_view property_name) {
  return magic_enum::enum_cast<E>(
             json.at(std::string{property_name}).as_string())
      .value_or(E::kInvalid);
}
}  // namespace

namespace stonks {
template <>
std::optional<binance::PlaceOrderResult> ParseFromJson(
    const web::json::value& json) {
  try {
    return binance::PlaceOrderResult{
        .client_order_id = GetStringPropertyAsString(json, "clientOrderId"),
        .cummulative_quote_quantity =
            GetStringPropertyAsDouble(json, "cummulativeQuoteQty"),
        .executed_quantity = GetStringPropertyAsDouble(json, "executedQty"),
        // .fills
        .order_id = GetInt64PropertyAsInt64(json, "orderId"),
        .order_list_id = GetInt64PropertyAsInt64(json, "orderListId"),
        .original_quantity = GetStringPropertyAsDouble(json, "origQty"),
        .price = GetStringPropertyAsDouble(json, "price"),
        .side = GetStringPropertyAsEnum<binance::Side>(json, "side"),
        .status = GetStringPropertyAsEnum<binance::Status>(json, "status"),
        .symbol = GetStringPropertyAsString(json, "symbol"),
        .time_in_force =
            GetStringPropertyAsEnum<binance::TimeInForce>(json, "timeInForce"),
        .transaction_time = GetInt64PropertyAsInt64(json, "transactTime"),
        .type = GetStringPropertyAsEnum<binance::Type>(json, "type")};
  } catch (const std::exception& exeption) {
    spdlog::error("Parse from JSON: {}", exeption.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}
}  // namespace stonks