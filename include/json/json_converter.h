#ifndef STONKS_JSON_JSON_CONVERTER_H_
#define STONKS_JSON_JSON_CONVERTER_H_

#include <cpprest/json.h>

#include <any>
#include <chrono>
#include <exception>
#include <stdexcept>
#include <string>

#include "finance_types.h"
#include "json_conversions.h"

namespace stonks::json {
template <typename T>
class JsonConverter {
 public:
  /**
   * @throws If any stores wrong type.
   */
  [[nodiscard]] auto ConvertAnyToJson(const std::any &data) const
      -> web::json::value {
    try {
      return ::stonks::json::ConvertToJson(std::any_cast<T>(data));
    } catch (const std::exception &) {
      throw;
    }
  }

  /**
   * @throws If cannot parse object of the type from JSON.
   */
  [[nodiscard]] auto ParseAnyFromJson(const web::json::value &json) const
      -> std::any {
    const auto object = ::stonks::json::ParseFromJson<T>(json);

    if (!object.has_value()) {
      throw std::invalid_argument("Couldn't parse type from JSON");
    }

    // TODO(vh): is move called here? or should I add move?
    return *object;
  }
};

using JsonConverterVariant =
    std::variant<JsonConverter<std::string>, JsonConverter<int>,
                 JsonConverter<int64_t>, JsonConverter<double>,
                 JsonConverter<std::chrono::milliseconds>,
                 JsonConverter<finance::Symbol>>;
}  // namespace stonks::json

#endif  // STONKS_JSON_JSON_CONVERTER_H_
