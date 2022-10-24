#ifndef STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_
#define STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_

#include <string>
#include <tuple>

#include "sqldb_table_traits.h"
#include "sqldb_types.h"

namespace stonks::sqldb::qb {
namespace detail {
struct ColumnDefinition {
  std::string name{};
  DataTypeVariant type{};
  bool unique{};
};

struct PrimaryKey {
  std::string column_name{};
  bool auto_increment{};
};

struct ForeignKey {
  std::string column_name{};
  std::string target_table_name{};
  std::string target_column_name{};
};

template <typename T>
struct ColumnsTraits;

template <typename... Columns>
struct ColumnsTraits<std::tuple<Columns...>> {
  [[nodiscard]] static auto GetColumnDefinitions() -> auto & {
    static const auto kConstant = [] {
      auto values = std::vector<ColumnDefinition>{};
      GetColumnDefinitionsImpl<Columns...>(values);
      return values;
    }();
    return kConstant;
  }

  [[nodiscard]] static auto GetPrimaryKeys() -> auto & {
    static const auto kConstant = [] {
      auto values = std::vector<PrimaryKey>{};
      GetPrimaryKeysImpl<Columns...>(values);
      return values;
    }();
    return kConstant;
  }

  [[nodiscard]] static auto GetForeignKeys() -> auto & {
    static const auto kConstant = [] {
      auto values = std::vector<ForeignKey>{};
      GetForeignKeysImpl<Columns...>(values);
      return values;
    }();
    return kConstant;
  }

 private:
  template <typename Column, typename... OtherColumns>
  static void GetColumnDefinitionsImpl(std::vector<ColumnDefinition> &values) {
    using ColumnTraits = ColumnTraits<Column>;

    values.emplace_back(ColumnDefinition{.name = ColumnTraits::GetName(),
                                         .type = ColumnTraits::GetType(),
                                         .unique = ColumnTraits::IsUnique()});

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetColumnDefinitionsImpl<OtherColumns...>(values);
    }
  }

  template <typename Column, typename... OtherColumns>
  static void GetPrimaryKeysImpl(std::vector<PrimaryKey> &values) {
    using ColumnTraits = ColumnTraits<Column>;

    if constexpr (ColumnTraits::IsPrimaryKey()) {
      values.emplace_back(
          PrimaryKey{.column_name = ColumnTraits::GetName(),
                     .auto_increment = ColumnTraits::HasAutoIncrement()});
    }

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetPrimaryKeysImpl<OtherColumns...>(values);
    }
  }

  template <typename Column, typename... OtherColumns>
  static void GetForeignKeysImpl(std::vector<ForeignKey> &values) {
    using ColumnTraits = ColumnTraits<Column>;

    if constexpr (ColumnTraits::IsForeignKey()) {
      values.emplace_back(ForeignKey{
          .column_name = ColumnTraits::GetName(),
          .target_table_name =
              TableTraits<typename Column::ForeignKey::Table>::GetName(),
          .target_column_name = ::stonks::sqldb::ColumnTraits<
              typename Column::ForeignKey>::GetName()});
    }

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetForeignKeysImpl<OtherColumns...>(values);
    }
  }
};
}  // namespace detail

/**
 * @brief Builds create table query.
 */
class Create {
 public:
  /**
   * @tparam Table Table definition.
   */
  template <typename Table, typename ColumnsTraits =
                                detail::ColumnsTraits<typename Table::Columns>>
  explicit Create(Table * /*unused*/)
      : Create{TableTraits<Table>::GetName(),
               ColumnsTraits::GetColumnDefinitions(),
               ColumnsTraits::GetPrimaryKeys(),
               ColumnsTraits::GetForeignKeys()} {}

  /**
   * @brief Adds if-not-exists condition.
   */
  [[nodiscard]] auto IfNotExists() -> Create &;

  /**
   * @brief Builds the query from inputs.
   */
  [[nodiscard]] auto Build() const -> Query;

 private:
  Create(std::string table_name,
         const std::vector<detail::ColumnDefinition> &column_definitions,
         const std::vector<detail::PrimaryKey> &primary_keys,
         const std::vector<detail::ForeignKey> &foreign_keys);

  std::string table_name_{};
  std::string columns_query_{};
  std::string primary_keys_query_{};
  std::string foreign_keys_query_{};
  std::string if_not_exists_query_{};
};
}  // namespace stonks::sqldb::qb

#endif  // STONKS_SQLDB_QUERY_BUILDER_SQLDB_QB_CREATE_H_
