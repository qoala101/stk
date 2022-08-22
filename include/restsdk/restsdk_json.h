#ifndef STONKS_RESTSDK_RESTSDK_JSON_H_
#define STONKS_RESTSDK_RESTSDK_JSON_H_

#include <string>
#include <string_view>

#include "cpp_not_null.h"
#include "cpp_polymorphic_value.h"
#include "network_i_json.h"
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
   * @copydoc network::IJson::clone
   */
  [[nodiscard]] auto clone() const -> cpp::NnUp<IJson> override;

  /**
   * @copydoc network::IJson::GetChild
   */
  [[nodiscard]] auto GetChild(
      std::string_view key) const& -> cpp::Pv<IJson> override;

  /**
   * @copydoc network::IJson::GetChild
   */
  [[nodiscard]] auto GetChild(
      std::string_view key) && -> cpp::Pv<IJson> override;

  /**
   * @copydoc network::IJson::SetChild
   */
  void SetChild(std::string key, const IJson& child) override;

  /**
   * @copydoc network::IJson::SetChild
   */
  void SetChild(std::string key, IJson&& child) override;

  /**
   * @copydoc network::IJson::GetImpl
   */
  [[nodiscard]] auto GetImpl() const -> const IJson::Impl& override;

  /**
   * @copydoc network::IJson::GetImpl
   */
  [[nodiscard]] auto GetImpl() -> IJson::Impl& override;

 private:
  network::IJson::Impl impl_;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_JSON_H_
