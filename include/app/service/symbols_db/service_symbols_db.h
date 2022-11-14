#ifndef STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SYMBOLS_DB_H_
#define STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SYMBOLS_DB_H_

#include <absl/time/time.h>

#include <callable.hpp>  // IWYU pragma: keep
#include <utility>
#include <vector>

#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_optional.h"
#include "member_function.hpp"
#include "network_auto_parsable_request.h"
#include "network_rest_client.h"
#include "network_typed_endpoint.h"

namespace stonks::service {
template <typename T>
class Connection;

template <cpp::MemberFunction auto F>
struct Endpoint {
  static constexpr auto Function = F;
  using FunctionType = decltype(Function);

  auto operator() [[nodiscard]] () -> network::TypedEndpoint;
};

template <cpp::MemberFunction auto F>
struct Handler {
  static constexpr auto Function = F;
  using FunctionType = decltype(Function);

  using Parent = typename member_function_traits<FunctionType>::class_type;
  using Result = typename member_function_traits<FunctionType>::return_type;

  Handler(Parent &parent, network::AutoParsableRestRequest request)
      : parent_{parent}, request_{std::move(request)} {}

  auto operator() [[nodiscard]] () -> Result;

  Parent &parent_;
  network::AutoParsableRestRequest request_;
};

/**
 * @copydoc core::ISymbolsDb
 */
template <>
class Connection<core::ISymbolsDb> : public core::ISymbolsDb {
 public:
  explicit Connection(network::RestClient rest_client)
      : rest_client_{std::move(rest_client)} {}

  /**
   * @copydoc core::ISymbolsDb::SelectAssets
   */
  auto SelectAssets [[nodiscard]] () const -> std::vector<core::Asset> override;

  /**
   * @copydoc core::ISymbolsDb::UpdateAssets
   */
  void UpdateAssets(std::vector<core::Asset> assets) override;

  /**
   * @copydoc core::ISymbolsDb::SelectSymbolsWithPriceRecords
   */
  auto SelectSymbolsWithPriceRecords [[nodiscard]] () const
      -> std::vector<core::Symbol> override;

  /**
   * @copydoc core::ISymbolsDb::SelectSymbolInfo
   */
  auto SelectSymbolInfo [[nodiscard]] (core::Symbol symbol) const
      -> cpp::Opt<core::SymbolInfo> override;

  /**
   * @copydoc core::ISymbolsDb::SelectSymbolsInfo
   */
  auto SelectSymbolsInfo [[nodiscard]] () const
      -> std::vector<core::SymbolInfo> override;

  /**
   * @copydoc core::ISymbolsDb::UpdateSymbolsInfo
   */
  void UpdateSymbolsInfo(std::vector<core::SymbolInfo> infos) override;

  /**
   * @copydoc core::ISymbolsDb::SelectSymbolPriceRecords
   */
  auto SelectSymbolPriceRecords
      [[nodiscard]] (const SelectSymbolPriceRecordsArgs &args) const
      -> std::vector<core::SymbolPriceRecord> override;

  /**
   * @copydoc core::ISymbolsDb::InsertSymbolPriceRecord
   */
  void InsertSymbolPriceRecord(core::SymbolPriceRecord record) override;

  /**
   * @copydoc core::ISymbolsDb::DeleteSymbolPriceRecords
   */
  void DeleteSymbolPriceRecords(absl::Time before_time) override;

 private:
  network::RestClient rest_client_;
};
}  // namespace stonks::service

#endif  // STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SYMBOLS_DB_H_
