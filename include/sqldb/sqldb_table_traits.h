#ifndef STONKS_SQLDB_SQLDB_TABLE_TRAITS_H_
#define STONKS_SQLDB_SQLDB_TABLE_TRAITS_H_

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
  [[nodiscard]] static consteval auto IsTuple() { return false; }
};

template <typename... Ts>
struct TupleTraits<std::tuple<Ts...>> {
  [[nodiscard]] static consteval auto IsTuple() { return true; }
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

/**
 * @brief Alias for either column or table which has a special treatment.
 */
template <typename Original, typename Alias>
struct As {};

/**
 * @brief Information retrieved from the column declaration.
 */
template <typename Column>
struct ColumnTraits {
  /**
   * @brief Gives short column name which is the name of its type.
   */
  [[nodiscard]] static constexpr auto GetName() -> auto & {
    return cpp::NameOf<Column>();
  }

  /**
   * @brief Gives full column name which includes its table name.
   */
  [[nodiscard]] static auto GetFullName() -> auto & {
    static const auto kConstant =
        fmt::format("{}.{}", cpp::NameOf<typename Column::Table>(), GetName());
    return kConstant;
  }

  /**
   * @brief Gives column data type.
   */
  [[nodiscard]] static consteval auto GetType() {
    return DataTypeVariant{DataType<typename Column::DataType>{}};
  }

  /**
   * @brief Tells whether column is part of the primary key.
   */
  [[nodiscard]] static consteval auto IsPrimaryKey() {
    return requires { typename Column::PrimaryKey; };
  }

  /**
   * @brief Tells whether column is foreign key.
   */
  [[nodiscard]] static consteval auto IsForeignKey() {
    return requires { typename Column::ForeignKey; };
  }

  /**
   * @brief Tells whether column has auto increment.
   */
  [[nodiscard]] static consteval auto HasAutoIncrement() {
    return requires { typename Column::AutoIncrement; };
  }

  /**
   * @brief Tells whether column data is unique.
   */
  [[nodiscard]] static consteval auto IsUnique() {
    return requires { typename Column::Unique; };
  }
};

/**
 * @brief Traits for the AS alias which are mostly the same
 * as the original column ones.
 */
template <typename Original, typename Alias>
struct ColumnTraits<As<Original, Alias>> : public ColumnTraits<Original> {
  /**
   * @copydoc ColumnTraits::GetName
   */
  [[nodiscard]] static constexpr auto GetName() -> auto & {
    return ColumnTraits<Alias>::GetName();
  }

  /**
   * @copydoc ColumnTraits::GetFullName
   */
  [[nodiscard]] static auto GetFullName() -> auto & {
    static const auto kConstant =
        fmt::format("{} AS {}", ColumnTraits<Original>::GetFullName(),
                    ColumnTraits<Original>::GetName());
    return kConstant;
  }
};

template <typename T>
struct ColumnsTraits;

template <typename... Columns>
struct ColumnsTraits<std::tuple<Columns...>> {
  /**
   * @brief Gives number of columns.
   */
  [[nodiscard]] static consteval auto GetSize() { return sizeof...(Columns); }

  /**
   * @brief Get the Names object
   *
   * @return auto&
   */
  [[nodiscard]] static constexpr auto GetNames() -> auto & {
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
      auto types = std::vector<sqldb::ColumnType>{};
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
  static void GetTypesImpl(std::vector<sqldb::ColumnType> &types) {
    types.emplace_back(
        sqldb::ColumnType{.column = {ColumnTraits<Column>::GetName()},
                          .type = {ColumnTraits<Column>::GetType()}});

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetTypesImpl<OtherColumns...>(types);
    }
  }
};

/**
 * @brief Information retrieved from the table declaration.
 */
template <typename Table>
struct TableTraits {
  /**
   * @brief Gives table name which is the name of the type.
   */
  [[nodiscard]] static constexpr auto GetName() -> auto & {
    return cpp::NameOf<Table>();
  }
};

/**
 * @brief Traits for the AS alias which are mostly the same
 * as the original table ones.
 */
template <typename Original, typename Alias>
struct TableTraits<As<Original, Alias>> : public TableTraits<Original> {
  /**
   * @copydoc TableTraits::GetName
   */
  [[nodiscard]] static auto GetName() -> auto & {
    static const auto kConstant =
        fmt::format("{} AS {}", TableTraits<Original>::GetName(),
                    TableTraits<Alias>::GetName());
    return kConstant;
  }
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_TABLE_TRAITS_H_
