#ifndef STONKS_SQLDB_SQLDB_TRAITS_H_
#define STONKS_SQLDB_SQLDB_TRAITS_H_

#include <fmt/format.h>

#include <gsl/assert>
#include <string>
#include <type_traits>

#include "cpp_name_of.h"
#include "cpp_optional.h"
#include "sqldb_types.h"

/**
 * @file API to get info from the table and column types.
 */

namespace stonks::sqldb {
namespace detail {
template <typename T>
struct TupleTraits {
  [[nodiscard]] static constexpr auto IsTuple() { return false; }
};

template <typename... Ts>
struct TupleTraits<std::tuple<Ts...>> {
  [[nodiscard]] static constexpr auto IsTuple() { return true; }
};
}  // namespace detail

template <typename T>
concept Tuple = requires { detail::TupleTraits<T>::IsTuple(); };

template <typename T>
concept TableT = requires {
                   true;
                   // detail::TupleTraits<typename T::Columns>::IsTuple();
                 };

template <typename T>
concept ColumnT = requires {
                    true;
                    // TableT<typename T::Table>;
                    // typename T::DataType;
                  };

namespace detail {
template <ColumnT Column>
[[nodiscard]] consteval auto HasPrimaryKey() {
  return requires { typename Column::PrimaryKey; };
}

template <ColumnT Column>
[[nodiscard]] consteval auto HasAutoIncrement() {
  return requires { typename Column::AutoIncrement; };
}

template <ColumnT Column>
[[nodiscard]] consteval auto HasUnique() {
  return requires { typename Column::Unique; };
}

template <ColumnT Column>
[[nodiscard]] consteval auto HasForeignKey() {
  return requires { typename Column::ForeignKey; };
}
}  // namespace detail

template <typename Original, typename Alias>
struct As {};

template <ColumnT Column>
struct ColumnTraits {
  [[nodiscard]] static auto GetName() -> auto & {
    return cpp::NameOf<Column>();
  }

  [[nodiscard]] static auto GetFullName() -> auto & {
    static const auto kConstant =
        fmt::format("{}.{}", cpp::NameOf<typename Column::Table>(), GetName());
    return kConstant;
  }

  [[nodiscard]] static constexpr auto GetType() {
    using Type = typename Column::DataType;

    if constexpr (std::is_same_v<Type, bool>) {
      return sqldb::DataType::kBool;
    } else if constexpr (std::is_same_v<Type, int>) {
      return sqldb::DataType::kInt;
    } else if constexpr (std::is_same_v<Type, int64_t>) {
      return sqldb::DataType::kInt64;
    } else if constexpr (std::is_same_v<Type, double>) {
      return sqldb::DataType::kDouble;
    } else if constexpr (std::is_same_v<Type, std::string>) {
      return sqldb::DataType::kString;
    } else {
      Expects(false);
    }
  }
};

template <ColumnT Original, ColumnT Alias>
struct ColumnTraits<As<Original, Alias>> {
  [[nodiscard]] static constexpr auto GetName() -> auto & {
    return ColumnTraits<Alias>::GetName();
  }

  [[nodiscard]] static auto GetFullName() -> auto & {
    static const auto kName = fmt::format(
        "{} AS {}", ColumnTraits<Original>::GetFullName(), GetName());
    return kName;
  }

  [[nodiscard]] static constexpr auto GetForeignKey() -> auto & {
    return ColumnTraits<Original>::GetForeignKey();
  }

  [[nodiscard]] static constexpr auto GetType() {
    return ColumnTraits<Original>::GetType();
  }

  [[nodiscard]] static constexpr auto GetDefinition() -> auto & {
    return ColumnTraits<Original>::GetDefinition();
  }
};

template <typename T>
struct ColumnsTraits;

template <typename... Columns>
struct ColumnsTraits<std::tuple<Columns...>> {
  [[nodiscard]] static constexpr auto GetSize() { return sizeof...(Columns); }

  [[nodiscard]] static auto GetNames() -> auto & {
    static const auto kValue = [] {
      auto names = std::string{};
      GetNamesImpl<Columns...>(names);
      return names;
    }();
    return kValue;
  }

  [[nodiscard]] static auto GetFullNames() -> auto & {
    static const auto kValue = [] {
      auto names = std::string{};
      GetFullNamesImpl<Columns...>(names);
      return names;
    }();
    return kValue;
  }

  [[nodiscard]] static auto GetTypes() -> auto & {
    static const auto kValue = [] {
      auto types = std::vector<sqldb::CellDefinition>{};
      GetTypesImpl<Columns...>(types);
      return types;
    }();
    return kValue;
  }

 private:
  template <ColumnT Column, ColumnT... OtherColumns>
  static void GetNamesImpl(std::string &names) {
    if (names.empty()) {
      names = ColumnTraits<Column>::GetName();
    } else {
      names += fmt::format(", {}", ColumnTraits<Column>::GetName());
    }

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetNamesImpl<OtherColumns...>(names);
    }
  }

  template <ColumnT Column, ColumnT... OtherColumns>
  static void GetFullNamesImpl(std::string &names) {
    if (names.empty()) {
      names = ColumnTraits<Column>::GetFullName();
    } else {
      names += fmt::format(", {}", ColumnTraits<Column>::GetFullName());
    }

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetFullNamesImpl<OtherColumns...>(names);
    }
  }

  template <ColumnT Column, ColumnT... OtherColumns>
  static void GetTypesImpl(std::vector<sqldb::CellDefinition> &types) {
    types.emplace_back(
        sqldb::CellDefinition{.column = {ColumnTraits<Column>::GetName()},
                              .data_type = ColumnTraits<Column>::GetType()});

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetTypesImpl<OtherColumns...>(types);
    }
  }
};

template <TableT Table>
struct TableTraits {
  [[nodiscard]] static auto GetName() -> auto & {
    static const auto kValue = std::string{nameof::nameof_short_type<Table>()};
    return kValue;
  }
};

template <TableT Original, TableT Alias>
struct TableTraits<As<Original, Alias>> {
  [[nodiscard]] static auto GetName() -> auto & {
    static const auto kName =
        fmt::format("{} AS {}", TableTraits<Original>::GetName(),
                    TableTraits<Alias>::GetName());
    return kName;
  }

  [[nodiscard]] static constexpr auto GetDefinition() -> auto & {
    return TableTraits<Original>::GetDefinition();
  }
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_TRAITS_H_
