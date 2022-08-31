#include "nosqldb_i_factory.h"

#include <memory>
#include <utility>

#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "nosqldb_i_db.h"
#include "nosqldb_i_items_interface.h"
#include "nosqldb_i_tables_interface.h"
#include "nosqldb_types.h"
#include "not_null.hpp"

namespace stonks::nosqldb {
namespace {
class AggregateDb : public IDb {
 public:
  AggregateDb(cpp::NnUp<ITablesInterface> tables_interface,
              cpp::NnUp<IItemsInterface> items_interface)
      : tables_interface_{std::move(tables_interface)},
        items_interface_{std::move(items_interface)} {}

  void CreateTableIfNotExists(const Table &table) override {
    return tables_interface_->CreateTableIfNotExists(table);
  }

  void DropTableIfExists(const Table &table) override {
    return tables_interface_->DropTableIfExists(table);
  }

  [[nodiscard]] auto SelectItem(const Table &table, const Key &key) const
      -> cpp::Opt<Item> override {
    return items_interface_->SelectItem(table, key);
  }

  void InsertOrUpdateItem(const Table &table, Item item) override {
    return items_interface_->InsertOrUpdateItem(table, std::move(item));
  }

  void DeleteItemIfExists(const Table &table, const Key &key) override {
    return items_interface_->DeleteItemIfExists(table, key);
  }

 private:
  cpp::NnUp<ITablesInterface> tables_interface_;
  cpp::NnUp<IItemsInterface> items_interface_;
};
}  // namespace

auto IFactory::CreateDb() const -> cpp::NnUp<IDb> {
  return cpp::MakeNnUp<AggregateDb>(CreateTablesInterface(),
                                    CreateItemsInterface());
}
};  // namespace stonks::nosqldb