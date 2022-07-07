#ifndef STONKS_NETWORK_JSON_H_
#define STONKS_NETWORK_JSON_H_

#include <exception>

// #include "json_conversions.h"

namespace stonks::network {
class Json {
 public:
  explicit Json() = default;

  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  // Json(web::json::value json) : json_{std::move(json)} {}

  template <typename T>
  // NOLINTNEXTLINE(*-explicit-constructor, *-explicit-conversions)
  Json(const T &t)
  // : json_{json::ConvertToJson(t)}
  {}

  template <typename T>
  [[nodiscard]] auto Parse() -> T {
    // return json::ParseFromJson<T>(json_);
  }

  template <typename T>
  explicit operator T() {
    // return json::ParseFromJson<T>(json_);
  }

  // [[nodiscard]] auto GetJson() const -> const web::json::value & {
  //   return json_;
  // }

 private:
  // web::json::value json_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_JSON_H_
