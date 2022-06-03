#ifndef STONKS_JSON_TYPE_VARIANT_H_
#define STONKS_JSON_TYPE_VARIANT_H_

#include <chrono>
#include <optional>
#include <string>

#include "any.h"
#include "binance_types.h"
#include "finance_types.h"
#include "ngrok_types.h"
#include "type.h"

namespace stonks::json {
/**
 * @brief Stores type info, and used for any conversions.
 */
class TypeVariant {
 public:
  explicit TypeVariant() = default;

  template <typename T>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  TypeVariant(Type<T> type) : type_{type} {}

  [[nodiscard]] constexpr auto IsOptional() const -> bool {
    return std::visit([](const auto &variant) { return variant.IsOptional(); },
                      type_);
  }

  [[nodiscard]] auto ParseAnyFromJson(const web::json::value &json) const
      -> json::Any;

  [[nodiscard]] auto ConvertAnyToJson(const json::Any &data) const
      -> std::optional<web::json::value>;

  [[nodiscard]] auto MakeNulloptAny() const -> json::Any;

 private:
  std::variant<
      Type<bool>, Type<int>, Type<int64_t>, Type<double>, Type<std::string>,
      Type<std::chrono::milliseconds>, Type<std::runtime_error>,
      Type<finance::Symbol>, Type<binance::ExchangeInfo>,
      Type<finance::SymbolPriceTick>, Type<finance::SymbolBalanceTick>,
      Type<finance::Period>, Type<ngrok::Tunnel>,

      Type<std::vector<std::string>>, Type<std::vector<finance::Symbol>>,
      Type<std::vector<finance::SymbolPriceTick>>,
      Type<std::vector<finance::SymbolBalanceTick>>,
      Type<std::vector<finance::SymbolInfo>>,

      Type<std::optional<int>>, Type<std::optional<finance::Period>>,
      Type<std::optional<std::vector<std::string>>>,
      Type<std::optional<std::vector<finance::Symbol>>>,
      Type<std::optional<std::vector<finance::SymbolPriceTick>>>>
      type_{};
};
}  // namespace stonks::json

#endif  // STONKS_JSON_TYPE_VARIANT_H_
