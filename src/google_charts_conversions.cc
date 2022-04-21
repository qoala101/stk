#include "google_charts_conversions.h"

#include <range/v3/to_container.hpp>
#include <range/v3/view/transform.hpp>

#include "concepts.h"
#include "json_conversions.h"

namespace stonks::google_charts {
namespace {
void AddColumn(std::vector<web::json::value> &columns, std::string_view label,
               std::string_view type) {
  auto &column = columns.emplace_back();
  column["label"] = web::json::value::string(std::string{label});
  column["type"] = web::json::value::string(std::string{type});
}

template <Number T>
void AddCell(std::vector<web::json::value> &cells, T value) {
  auto &cell = cells.emplace_back();
  cell["v"] = web::json::value::number(value);
}

void AddCell(std::vector<web::json::value> &cells, std::string_view value) {
  auto &cell = cells.emplace_back();
  cell["v"] = web::json::value::string(std::string{value});
}

void AddCell(std::vector<web::json::value> &cells,
             std::chrono::milliseconds value) {
  AddCell(cells, value.count());
}

void AddCell(std::vector<web::json::value> &cells,
             const web::json::value &value) {
  AddCell(cells, value.serialize());
}
}  // namespace

web::json::value ConvertToJson(const std::vector<finance::Candle> &data) {
  auto cols = std::vector<web::json::value>{};
  cols.reserve(7);
  AddColumn(cols, "Time", "string");
  AddColumn(cols, "High", "number");
  AddColumn(cols, "Open", "number");
  AddColumn(cols, "Close", "number");
  AddColumn(cols, "Low", "number");
  AddColumn(cols, "AA", "number");  // TODO
  AddColumn(cols, "BB", "number");  // TODO

  const auto candle_to_json = [](const finance::Candle &candle) {
    auto json = web::json::value{};
    auto row_cells = std::vector<web::json::value>{};
    row_cells.reserve(7);
    AddCell(row_cells, candle.open_time);
    AddCell(row_cells, candle.data.value_or(finance::Candle::Data{}).low_price);
    AddCell(row_cells,
            candle.data.value_or(finance::Candle::Data{}).open_price);
    AddCell(row_cells,
            candle.data.value_or(finance::Candle::Data{}).close_price);
    AddCell(row_cells,
            candle.data.value_or(finance::Candle::Data{}).high_price);
    AddCell(row_cells,
            candle.data.value_or(finance::Candle::Data{}).high_price);  // TODO
    AddCell(row_cells,
            candle.data.value_or(finance::Candle::Data{}).low_price);  // TODO
    json["c"] = web::json::value::array(std::move(row_cells));
    return json;
  };
  auto rows =
      data | ranges::views::transform(candle_to_json) | ranges::to_vector;

  auto json = web::json::value{};
  json["cols"] = web::json::value::array(std::move(cols));
  json["rows"] = web::json::value::array(std::move(rows));
  return json;
}

web::json::value ConvertToJson(
    const std::vector<finance::StrategyOrderRequest> &data) {
  auto cols = std::vector<web::json::value>{};
  cols.reserve(3);
  AddColumn(cols, "Time", "number");
  AddColumn(cols, "ETH", "number");
  AddColumn(cols, "USDT", "number");

  const auto record_to_json = [](const finance::StrategyOrderRequest &record) {
    auto json = web::json::value{};
    auto row_cells = std::vector<web::json::value>{};
    row_cells.reserve(3);
    // AddCell(row_cells, record.order_request.time);
    // AddCell(row_cells, record.order_request.quantity);
    // AddCell(row_cells, record.order_request.price);
    json["c"] = web::json::value::array(std::move(row_cells));
    return json;
  };
  auto rows =
      data | ranges::views::transform(record_to_json) | ranges::to_vector;

  auto json = web::json::value{};
  json["cols"] = web::json::value::array(std::move(cols));
  json["rows"] = web::json::value::array(std::move(rows));
  return json;
}

web::json::value ConvertToJson(const std::vector<finance::TimeDouble> &data) {
  auto cols = std::vector<web::json::value>{};
  cols.reserve(2);
  AddColumn(cols, "Time", "number");
  AddColumn(cols, "Value", "number");

  const auto time_double_to_json = [](const finance::TimeDouble &record) {
    auto json = web::json::value{};
    auto row_cells = std::vector<web::json::value>{};
    row_cells.reserve(2);
    AddCell(row_cells, record.time);
    AddCell(row_cells, record.value);
    json["c"] = web::json::value::array(std::move(row_cells));
    return json;
  };
  auto rows =
      data | ranges::views::transform(time_double_to_json) | ranges::to_vector;

  auto json = web::json::value{};
  json["cols"] = web::json::value::array(std::move(cols));
  json["rows"] = web::json::value::array(std::move(rows));
  return json;
}

web::json::value ConvertToJson(const std::vector<finance::Order> &data) {
  auto cols = std::vector<web::json::value>{};
  cols.reserve(8);
  AddColumn(cols, "request_time", "string");
  AddColumn(cols, "uuid", "string");
  AddColumn(cols, "symbol", "string");
  AddColumn(cols, "buy_or_sell", "string");
  AddColumn(cols, "amount", "string");
  AddColumn(cols, "order_type", "string");
  AddColumn(cols, "order_updates", "string");
  AddColumn(cols, "strategy_data", "string");

  const auto item_to_json = [](const finance::Order &item) {
    auto json = web::json::value{};
    auto row_cells = std::vector<web::json::value>{};
    row_cells.reserve(8);
    AddCell(row_cells, json::ConvertToJson(item.request_time));
    AddCell(row_cells, json::ConvertToJson(item.uuid));
    AddCell(row_cells, json::ConvertToJson(item.symbol));
    AddCell(row_cells, json::ConvertToJson(item.buy_or_sell));
    AddCell(row_cells, json::ConvertToJson(item.amount));
    AddCell(row_cells, json::ConvertToJson(item.order_type));
    AddCell(row_cells, json::ConvertToJson(item.order_updates));
    AddCell(row_cells, json::ConvertToJson(item.strategy_data));
    json["c"] = web::json::value::array(std::move(row_cells));
    return json;
  };
  auto rows = data | ranges::views::transform(item_to_json) | ranges::to_vector;

  auto json = web::json::value{};
  json["cols"] = web::json::value::array(std::move(cols));
  json["rows"] = web::json::value::array(std::move(rows));
  return json;
}
}  // namespace stonks::google_charts