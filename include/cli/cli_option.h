#ifndef STONKS_CLI_CLI_OPTION_H_
#define STONKS_CLI_CLI_OPTION_H_

#include "cli_concepts.h"
#include "cli_types.h"
#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_not_null.h"
#include "cpp_this.h"

namespace stonks::cli {
namespace detail {
class OptionBase {
 public:
  explicit OptionBase(cpp::NnSp<OptionValueVariant> value);

 protected:
  auto GetValue [[nodiscard]] () const -> const OptionValueVariant&;
  auto GetValue [[nodiscard]] () -> OptionValueVariant&;

 private:
  static auto GetValueImpl [[nodiscard]] (cpp::This<OptionBase> auto& t)
  -> auto&;

  cpp::NnSp<OptionValueVariant> value_;
};
}  // namespace detail

/**
 * @brief Command line option.
 */
template <SupportedOptionType T>
class Option : public detail::OptionBase {
 public:
  using OptionBase::OptionBase;

  /**
   * @brief Gives option value.
   */
  auto operator* [[nodiscard]] () const -> auto& {
    return OperatorAsteriskImpl(*this);
  }

  auto operator* [[nodiscard]] () -> auto& {
    return OperatorAsteriskImpl(*this);
  }

 private:
  static auto OperatorAsteriskImpl [[nodiscard]] (cpp::This<Option> auto& t)
  -> auto& {
    return std::get<T>(*t.GetValue());
  }
};
}  // namespace stonks::cli

#endif  // STONKS_CLI_CLI_OPTION_H_