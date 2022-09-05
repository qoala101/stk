#ifndef STONKS_RESTSDK_RESTSDK_JSON_H_
#define STONKS_RESTSDK_RESTSDK_JSON_H_

#include <cstdint>
#include <string>
#include <string_view>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_copy_const.h"
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
  /**
   * @brief Constructs null JSON.
   */
  Json() = default;

  explicit Json(bool value);
  explicit Json(int value);
  explicit Json(int64_t value);
  explicit Json(double value);
  explicit Json(const char* value);
  explicit Json(std::string_view value);
  explicit Json(IJson::Impl impl);

  /**
   * @copydoc network::IJson::clone
   */
  [[nodiscard]] auto clone() const& -> cpp::NnUp<IJson> override;

  /**
   * @copydoc network::IJson::clone
   */
  [[nodiscard]] auto clone() && -> cpp::NnUp<IJson> override;

  /**
   * @copydoc network::IJson::IsNull
   */
  [[nodiscard]] auto IsNull() const -> bool override;

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
  void SetChild(std::string key, cpp::Pv<IJson> child) override;

  /**
   * @copydoc network::IJson::GetChild
   */
  [[nodiscard]] auto GetChild(int index) const& -> cpp::Pv<IJson> override;

  /**
   * @copydoc network::IJson::GetChild
   */
  [[nodiscard]] auto GetChild(int index) && -> cpp::Pv<IJson> override;

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
  [[nodiscard]] auto GetImpl() const -> const IJson::Impl& override;

  /**
   * @copydoc network::IJson::GetImpl
   */
  [[nodiscard]] auto GetImpl() -> IJson::Impl& override;

 private:
  [[nodiscard]] static auto CloneImpl(cpp::DecaysTo<Json> auto&& t)
      -> cpp::NnUp<IJson>;

  [[nodiscard]] static auto GetChildImpl(cpp::DecaysTo<Json> auto&& t,
                                         std::string_view key)
      -> cpp::Pv<IJson>;

  [[nodiscard]] static auto GetChildImpl(cpp::DecaysTo<Json> auto&& t,
                                         int index) -> cpp::Pv<IJson>;

  [[nodiscard]] static auto GetImplImpl(cpp::DecaysTo<Json> auto&& t)
      -> cpp::CopyConst<decltype(t), IJson::Impl&>;

  network::IJson::Impl impl_;
};
}  // namespace stonks::restsdk

#endif  // STONKS_RESTSDK_RESTSDK_JSON_H_
