#include "google_charts_conversions.h"

#include <range/v3/to_container.hpp>
#include <range/v3/view/transform.hpp>

#include "concepts.h"

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
}  // namespace

namespace stonks {
web::json::value ConvertToJson(const std::vector<Candlestick> &data) {
  auto cols = std::vector<web::json::value>{};
  cols.reserve(5);
  AddColumn(cols, "Time", "string");
  AddColumn(cols, "High", "number");
  AddColumn(cols, "Open", "number");
  AddColumn(cols, "Close", "number");
  AddColumn(cols, "Low", "number");
  AddColumn(cols, "AA", "number"); // TODO
  AddColumn(cols, "BB", "number"); // TODO

  const auto candlestick_to_json = [](const Candlestick &candlestick) {
    auto json = web::json::value{};
    auto row_cells = std::vector<web::json::value>{};
    row_cells.reserve(7);
    AddCell(row_cells, candlestick.open_time);
    AddCell(row_cells, candlestick.low_price);
    AddCell(row_cells, candlestick.open_price);
    AddCell(row_cells, candlestick.close_price);
    AddCell(row_cells, candlestick.high_price);
    AddCell(row_cells, candlestick.high_price); // TODO
    AddCell(row_cells, candlestick.low_price); // TODO
    json["c"] = web::json::value::array(std::move(row_cells));
    return json;
  };
  auto rows =
      data | ranges::views::transform(candlestick_to_json) | ranges::to_vector;

  auto json = web::json::value{};
  json["cols"] = web::json::value::array(std::move(cols));
  json["rows"] = web::json::value::array(std::move(rows));
  return json;
}
}  // namespace stonks