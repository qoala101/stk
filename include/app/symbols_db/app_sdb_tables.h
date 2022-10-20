#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_

#include <fmt/format.h>

#include <gsl/assert>
#include <nameof.hpp>
#include <type_traits>

#include "cpp_optional.h"
#include "sqldb_types.h"

namespace stonks::app::sdb::tables {
/**
 * @copydoc core::Asset
 */
struct Asset {
  [[nodiscard]] static auto Definition() -> const sqldb::TableDefinition &;

  static constexpr auto kTable = "Asset";
  static constexpr auto kId = "id";
  static constexpr auto kName = "name";

  struct id {
    using DataType = int64_t;
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct name {
    using DataType = std::string;
    struct Unique;
  };

  using Columns = std::tuple<id, name>;
};

/**
 * @copydoc core::SymbolInfo
 */
struct SymbolInfo {
  [[nodiscard]] static auto Definition() -> const sqldb::TableDefinition &;

  static constexpr auto kTable = "SymbolInfo";
  static constexpr auto kId = "id";
  static constexpr auto kName = "name";
  static constexpr auto kBaseAssetId = "base_asset_id";
  static constexpr auto kBaseAssetMinAmount = "base_asset_min_amount";
  static constexpr auto kBaseAssetPriceStep = "base_asset_price_step";
  static constexpr auto kQuoteAssetId = "quote_asset_id";
  static constexpr auto kQuoteAssetMinAmount = "quote_asset_min_amount";
  static constexpr auto kQuoteAssetPriceStep = "quote_asset_price_step";

  struct id {
    using DataType = int64_t;
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct name {
    using DataType = std::string;
    struct Unique;
  };

  struct base_asset_id {
    using DataType = int64_t;

    struct ForeignKey {
      using Table = Asset;
      using Column = Table::id;
    };
  };

  struct base_asset_min_amount {
    using DataType = double;
  };

  struct base_asset_price_step {
    using DataType = double;
  };

  struct quote_asset_id {
    using DataType = int64_t;

    struct ForeignKey {
      using Table = Asset;
      using Column = Table::id;
    };
  };

  struct quote_asset_min_amount {
    using DataType = double;
  };

  struct quote_asset_price_step {
    using DataType = double;
  };

  using Columns = std::tuple<id, name, base_asset_id, base_asset_min_amount,
                             base_asset_price_step, quote_asset_id,
                             quote_asset_min_amount, quote_asset_price_step>;
};

/**
 * @copydoc core::SymbolPriceRecord
 */
struct SymbolPriceRecord {
  [[nodiscard]] static auto Definition() -> const sqldb::TableDefinition &;

  static constexpr auto kTable = "SymbolPriceRecord";
  static constexpr auto kSymbolId = "symbol_id";
  static constexpr auto kPrice = "price";
  static constexpr auto kTime = "time";

  struct symbol_id {
    using DataType = int64_t;

    struct ForeignKey {
      using Table = SymbolInfo;
      using Column = Table::id;
    };
  };

  struct price {
    using DataType = double;
  };

  struct time {
    using DataType = int64_t;
    struct Unique;
  };

  using Columns = std::tuple<symbol_id, price, time>;
};

template <typename Column>
[[nodiscard]] constexpr auto HasPrimaryKey() {
  return requires { typename Column::PrimaryKey; };
}

template <typename Column>
[[nodiscard]] constexpr auto HasAutoIncrement() {
  return requires { typename Column::AutoIncrement; };
}

template <typename Column>
[[nodiscard]] constexpr auto HasUnique() {
  return requires { typename Column::Unique; };
}

template <typename Table>
struct TableTraitsGetName {
  [[nodiscard]] static auto GetName() {
    static const auto kName = std::string{nameof::nameof_short_type<Table>()};
    return kName;
  }
};

template <typename Column>
struct ColumnTraits {
  [[nodiscard]] static auto GetName() {
    static const auto kName = std::string{nameof::nameof_short_type<Column>()};
    return kName;
  }

  [[nodiscard]] static auto GetForeignKey() -> cpp::Opt<sqldb::ForeignKey> {
    if constexpr (requires { typename Column::ForeignKey; }) {
      return sqldb::ForeignKey{
          .table = {TableTraitsGetName<
              typename Column::ForeignKey::Table>::GetName()},
          .column = {
              ColumnTraits<typename Column::ForeignKey::Column>::GetName()}};
    } else {
      return std::nullopt;
    }
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

  [[nodiscard]] static auto GetDefinition() {
    return sqldb::ColumnDefinition{.column = {ColumnTraits<Column>::GetName()},
                                   .data_type = GetType(),
                                   .primary_key = HasPrimaryKey<Column>(),
                                   .auto_increment = HasAutoIncrement<Column>(),
                                   .unique = HasUnique<Column>(),
                                   .foreign_key = GetForeignKey()};
  }
};

template <typename T>
struct ColumnsTraits;

template <typename... Columns>
struct ColumnsTraits<std::tuple<Columns...>> {
  [[nodiscard]] static auto GetNames() {
    auto names = std::string{};
    GetNamesImpl<Columns...>(names);
    return names;
  }

  [[nodiscard]] static auto GetTypes() {
    auto types = std::vector<sqldb::CellDefinition>{};
    GetTypesImpl<Columns...>(types);
    return types;
  }

  [[nodiscard]] static auto GetDefinitions() {
    auto definitions = std::vector<sqldb::ColumnDefinition>{};
    GetDefinitionsImpl<Columns...>(definitions);
    return definitions;
  }

 private:
  template <typename Column, typename... OtherColumns>
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

  template <typename Column, typename... OtherColumns>
  static void GetTypesImpl(std::vector<sqldb::CellDefinition> &types) {
    types.emplace_back(
        sqldb::CellDefinition{.column = {ColumnTraits<Column>::GetName()},
                              .data_type = ColumnTraits<Column>::GetType()});

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetTypesImpl<OtherColumns...>(types);
    }
  }

  template <typename Column, typename... OtherColumns>
  static void GetDefinitionsImpl(
      std::vector<sqldb::ColumnDefinition> &definitions) {
    definitions.emplace_back(ColumnTraits<Column>::GetDefinition());

    if constexpr (sizeof...(OtherColumns) > 0) {
      GetDefinitionsImpl<OtherColumns...>(definitions);
    }
  }
};

template <typename Table>
struct TableTraits {
  [[nodiscard]] static auto GetName() -> auto & {
    static const auto kName = std::string{nameof::nameof_short_type<Table>()};
    return kName;
  }

  [[nodiscard]] static auto GetDefinition() -> auto & {
    static const auto kDefinition = sqldb::TableDefinition{
        .table = {GetName()},
        .columns = ColumnsTraits<typename Table::Columns>::GetDefinitions()};
    return kDefinition;
  }
};
}  // namespace stonks::app::sdb::tables

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_TABLES_H_
