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
  [[nodiscard]] static auto IsTuple() { return false; }
};

template <typename... Ts>
struct TupleTraits<std::tuple<Ts...>> {
  [[nodiscard]] static auto IsTuple() { return true; }
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
 * @brief Information retrieved from the table declaration.
 */
template <typename Table>
struct TableTraits {
  /**
   * @brief Gives table name which is the name of the type.
   */
  [[nodiscard]] static auto GetName() { return cpp::NameOf<Table>(); }
};

/**
 * @brief Information retrieved from the column declaration.
 */
template <typename Column>
struct ColumnTraits {
  /**
   * @brief Gives short column name which is the name of its type.
   */
  [[nodiscard]] static auto GetName() { return cpp::NameOf<Column>(); }

  /**
   * @brief Gives full column name which includes its table name.
   */
  [[nodiscard]] static auto GetFullName() {
    return fmt::format("{}.{}", cpp::NameOf<typename Column::Table>(),
                       GetName());
  }

  /**
   * @brief Gives column data type.
   */
  [[nodiscard]] static auto GetType() {
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
  [[nodiscard]] static auto HasAutoIncrement() {
    return requires { typename Column::AutoIncrement; };
  }

  /**
   * @brief Tells whether column data is unique.
   */
  [[nodiscard]] static auto IsUnique() {
    return requires { typename Column::Unique; };
  }
};
}  // namespace stonks::sqldb

#endif  // STONKS_SQLDB_SQLDB_TABLE_TRAITS_H_
