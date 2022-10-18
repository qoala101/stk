#ifndef STONKS_RESTSDK_RESTSDK_JSON_H_
#define STONKS_RESTSDK_RESTSDK_JSON_H_

#include <cstdint>
#include <string>
#include <string_view>

#include "cpp_not_null.h"
#include "cpp_polymorphic_value.h"
#include "cpp_this.h"  // IWYU pragma: keep
#include "network_i_json.h"
#include "restsdk_json_native_handle.h"

namespace stonks::restsdk {
/**
 * @copydoc network::IJson
 */
class Json : public network::IJson {
 public:
  /**
   * @brief Constructs null JSON.
   */
  Json() = default;

  explicit Json(bool value);
  explicit Json(int value);
  explicit Json(int64_t value);
  explicit Json(double value);
  explicit Json(std::string_view value);
  explicit Json(IJson::NativeHandle impl);

  /**
   * @copydoc network::IJson::clone
   */
  [[nodiscard]] auto clone() const -> cpp::NnUp<IJson> override;
  [[nodiscard]] auto clone() -> cpp::NnUp<IJson> override;

  /**
   * @copydoc network::IJson::IsNull
   */
  [[nodiscard]] auto IsNull() const -> bool override;

  /**
   * @copydoc network::IJson::GetChild
   */
  [[nodiscard]] auto GetChild(std::string_view key) const
      -> cpp::Pv<IJson> override;
  [[nodiscard]] auto GetChild(std::string_view key) -> cpp::Pv<IJson> override;

  /**
   * @copydoc network::IJson::SetChild
   */
  void SetChild(std::string key, cpp::Pv<IJson> child) override;

  /**
   * @copydoc network::IJson::GetChild
   */
  [[nodiscard]] auto GetChild(int index) const -> cpp::Pv<IJson> override;
  [[nodiscard]] auto GetChild(int index) -> cpp::Pv<IJson> override;

  /**
   * @copydoc network::IJson::SetChild
   */
  void SetChild(int index, cpp::Pv<IJson> child) override;

  /**
   * @copydoc network::IJson::GetSize
   */
  [[nodiscard]] auto GetSize() const -> int override;

  /**
   * @copydoc network::IJson::GetImpl
   */
  [[nodiscard]] auto GetNativeHandle() const
      -> const IJson::NativeHandle& override;
  [[nodiscard]] auto GetNativeHandle() -> IJson::NativeHandle& override;

 private:
  [[nodiscard]] static auto CloneImpl(cpp::This<Json> auto& t);

  [[nodiscard]] static auto GetChildImpl(cpp::This<Json> auto& t,
                                         std::string_view key);

  [[nodiscard]] static auto GetChildImpl(cpp::This<Json> auto& t, int index);

  [[nodiscard]] static auto GetNativeHandleImpl(cpp::This<Json> auto& t)
      -> auto&;

  network::IJson::NativeHandle native_handle_;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_JSON_H_
