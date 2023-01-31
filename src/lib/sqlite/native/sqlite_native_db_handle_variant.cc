#include "sqlite_native_db_handle_variant.h"

#include <gsl/assert>
#include <type_traits>
#include <variant>

#include "cpp_copy_const.h"
#include "sqlite_native_db_file_handle.h"

namespace vh::sqlite {
template <cpp::This<NativeDbHandleVariant> This>
auto NativeDbHandleVariant::GetNativeDbImpl(This &t) -> auto & {
  return std::visit(
      [](auto &v) -> cpp::CopyConst<This, sqlite3> & {
        using V = std::remove_cvref_t<decltype(v)>;

        if constexpr (std::is_same_v<V, NativeDbHandle>) {
          return *v;
        } else if constexpr (std::is_same_v<V, NativeDbFileHandle>) {
          return v.GetNativeDb();
        } else {
          Expects(false);
        }
      },
      *t);
}

auto NativeDbHandleVariant::GetNativeDb() const -> const sqlite3 & {
  return GetNativeDbImpl(*this);
}

auto NativeDbHandleVariant::GetNativeDb() -> sqlite3 & {
  return GetNativeDbImpl(*this);
}

auto NativeDbHandleVariant::GetFilePath() const -> const FilePath & {
  Expects(HasFilePath());
  return std::visit(
      [](const auto &v) -> const FilePath & {
        using V = std::remove_cvref_t<decltype(v)>;

        if constexpr (std::is_same_v<V, NativeDbFileHandle>) {
          return v.GetFilePath();
        } else {
          Expects(false);
        }
      },
      **this);
}

auto NativeDbHandleVariant::HasFilePath() const -> bool {
  return std::holds_alternative<NativeDbFileHandle>(**this);
}
}  // namespace vh::sqlite