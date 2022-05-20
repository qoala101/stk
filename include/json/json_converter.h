#ifndef STONKS_JSON_JSON_CONVERTER_H_
#define STONKS_JSON_JSON_CONVERTER_H_

#include <cpprest/json.h>

#include <any>
#include <chrono>
#include <exception>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "finance_types.h"
#include "json_conversions.h"

namespace stonks::json {
template <typename T>
class JsonConverter {
 public:
  using Type = T;
  using Optional = std::false_type;

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
      throw std::runtime_error{"Couldn't parse type from JSON"};
    }

    // TODO(vh): is move called here? or should I add move?
    return *object;
  }

  [[nodiscard]] auto MakeNulloptAny() const -> std::any {
    return std::optional<T>{std::nullopt};
  }
};

template <typename T>
class JsonConverter<std::optional<T>> {
 public:
  using Type = T;
  using Optional = std::true_type;

  /**
   * @throws If any stores wrong type.
   */
  [[nodiscard]] auto ConvertAnyToJson(const std::any &data) const
      -> web::json::value {
    if (!data.has_value()) {
      return web::json::value::null();
    }

    if (data.type() == typeid(std::optional<T>)) {
      return JsonConverter<T>{}.ConvertAnyToJson(
          *std::any_cast<std::optional<T>>(data));
    }

    return JsonConverter<T>{}.ConvertAnyToJson(data);
  }

  /**
   * @throws If cannot parse object of the type from JSON.
   */
  [[nodiscard]] auto ParseAnyFromJson(const web::json::value &json) const
      -> std::any {
    if (json.is_null()) {
      return {};
    }

    return std::optional<T>{
        std::any_cast<T>(JsonConverter<T>{}.ParseAnyFromJson(json))};
  }

  [[nodiscard]] auto MakeNulloptAny() const -> std::any {
    return JsonConverter<T>{}.MakeNulloptAny();
  }
};

using JsonConverterVariant = std::variant<
    JsonConverter<std::string>, JsonConverter<int>,
    JsonConverter<std::optional<int>>, JsonConverter<int64_t>,
    JsonConverter<double>, JsonConverter<std::chrono::milliseconds>,
    JsonConverter<finance::Symbol>, JsonConverter<std::runtime_error>,
    JsonConverter<std::vector<std::string>>,
    JsonConverter<std::optional<std::vector<std::string>>>,
    JsonConverter<std::vector<finance::Symbol>>,
    JsonConverter<std::optional<std::vector<finance::Symbol>>>,
    JsonConverter<finance::SymbolPriceTick>,
    JsonConverter<std::vector<finance::SymbolPriceTick>>,
    JsonConverter<std::optional<std::vector<finance::SymbolPriceTick>>>,
    JsonConverter<bool>, JsonConverter<finance::Period>,
    JsonConverter<std::optional<finance::Period>>>;
}  // namespace stonks::json

#endif  // STONKS_JSON_JSON_CONVERTER_H_
