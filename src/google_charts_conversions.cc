#include "google_charts_conversions.h"

#include <gsl/pointers>
#include <range/v3/to_container.hpp>
#include <range/v3/view/transform.hpp>

namespace stonks {
web::json::value ConvertToJson(const std::vector<Candlestick> &data) {
  auto cols = std::vector<web::json::value>{};
  cols.reserve(5);
  auto col = gsl::not_null{&cols.emplace_back()};
  (*col)["label"] = web::json::value::string("Time");
  (*col)["type"] = web::json::value::string("string");
  col = gsl::not_null{&cols.emplace_back()};
  (*col)["label"] = web::json::value::string("High");
  (*col)["type"] = web::json::value::string("number");
  col = gsl::not_null{&cols.emplace_back()};
  (*col)["label"] = web::json::value::string("Open");
  (*col)["type"] = web::json::value::string("number");
  col = gsl::not_null{&cols.emplace_back()};
  (*col)["label"] = web::json::value::string("Close");
  (*col)["type"] = web::json::value::string("number");
  col = gsl::not_null{&cols.emplace_back()};
  (*col)["label"] = web::json::value::string("Low");
  (*col)["type"] = web::json::value::string("number");

  const auto candlestick_to_json = [](const Candlestick &candlestick) {
    auto json = web::json::value{};
    auto row_cells = std::vector<web::json::value>{};
    row_cells.reserve(5);
    row_cells.emplace_back()["v"] =
        web::json::value::string(std::to_string(candlestick.open_time));
    row_cells.emplace_back()["v"] =
        web::json::value::number(candlestick.low_price);
    row_cells.emplace_back()["v"] =
        web::json::value::number(candlestick.open_price);
    row_cells.emplace_back()["v"] =
        web::json::value::number(candlestick.close_price);
    row_cells.emplace_back()["v"] =
        web::json::value::number(candlestick.high_price);
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