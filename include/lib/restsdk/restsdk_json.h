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

namespace vh::restsdk {
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
  auto clone [[nodiscard]] () const -> cpp::NnUp<IJson> override;
  auto clone [[nodiscard]] () -> cpp::NnUp<IJson> override;

  /**
   * @copydoc network::IJson::IsNull
   */
  auto IsNull [[nodiscard]] () const -> bool override;

  /**
   * @copydoc network::IJson::GetChild
   */
  auto GetChild [[nodiscard]] (std::string_view key) const
      -> cpp::Pv<IJson> override;
  auto GetChild [[nodiscard]] (std::string_view key) -> cpp::Pv<IJson> override;

  /**
   * @copydoc network::IJson::SetChild
   */
  void SetChild(std::string key, cpp::Pv<IJson> child) override;

  /**
   * @copydoc network::IJson::GetChild
   */
  auto GetChild [[nodiscard]] (int index) const -> cpp::Pv<IJson> override;
  auto GetChild [[nodiscard]] (int index) -> cpp::Pv<IJson> override;

  /**
   * @copydoc network::IJson::SetChild
   */
  void SetChild(int index, cpp::Pv<IJson> child) override;

  /**
   * @copydoc network::IJson::GetSize
   */
  auto GetSize [[nodiscard]] () const -> int override;

  /**
   * @copydoc network::IJson::GetImpl
   */
  auto GetNativeHandle [[nodiscard]] () const
      -> const IJson::NativeHandle& override;
  auto GetNativeHandle [[nodiscard]] () -> IJson::NativeHandle& override;

 private:
  static auto CloneImpl [[nodiscard]] (cpp::This<Json> auto& t);

  static auto GetChildImpl
      [[nodiscard]] (cpp::This<Json> auto& t, std::string_view key);

  static auto GetChildImpl [[nodiscard]] (cpp::This<Json> auto& t, int index);

  static auto GetNativeHandleImpl [[nodiscard]] (cpp::This<Json> auto& t)
  -> auto&;

  network::IJson::NativeHandle native_handle_;
};
}  // namespace vh::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_JSON_H_
