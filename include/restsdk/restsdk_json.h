#ifndef STONKS_RESTSDK_RESTSDK_JSON_H_
#define STONKS_RESTSDK_RESTSDK_JSON_H_

#include <memory>

#include "network_json.h"
#include "not_null.hpp"
#include "restsdk_json_impl.h"

namespace stonks::restsdk {
/**
 * @copydoc network::IJson
 */
class Json : public network::IJson {
 public:
  explicit Json() = default;
  explicit Json(network::IJson::Impl impl);

  /**
   * @copydoc network::IJson::Clone
   */
  [[nodiscard]] auto Clone() const
      -> cpp::not_null<std::unique_ptr<IJson>> override;

  [[nodiscard]] auto AsInt() const -> int override;
  void SetInt(int value) override;

  [[nodiscard]] auto AsInt64() const -> int64_t override;
  void SetInt64(int64_t value) override;

  [[nodiscard]] auto AsDouble() const -> double override;
  void SetDouble(double value) override;

  [[nodiscard]] auto AsString() const -> std::string override;
  void SetString(std::string_view value) override;

  [[nodiscard]] auto GetChild(std::string_view key) const
      -> cpp::not_null<std::unique_ptr<IJson>> override;
  void SetChild(std::string_view key, const IJson& child) override;

  /**
   * @copydoc network::IJson::GetImpl
   */
  [[nodiscard]] auto GetImpl() const -> const IJson::Impl& override;

 private:
  network::IJson::Impl impl_;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_JSON_H_
