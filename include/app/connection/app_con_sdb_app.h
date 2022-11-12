#ifndef STONKS_APP_ACTION_APP_A_SDB_APP_H_
#define STONKS_APP_ACTION_APP_A_SDB_APP_H_

#include <absl/time/time.h>

#include <callable.hpp>
#include <utility>
#include <vector>

#include "app_sdb_i_app.h"
#include "core_types.h"
#include "cpp_concepts.h"
#include "cpp_optional.h"
#include "network_auto_parsable_request.h"
#include "network_rest_client.h"
#include "network_rest_server_builder.h"
#include "network_typed_endpoint.h"

namespace stonks::app::a {
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
 * @copydoc IApp
 */
template <>
class Connection<sdb::IApp> : public sdb::IApp {
 public:
  /**
   * @copydoc IApp::SelectAssets
   */
  auto SelectAssets [[nodiscard]] () const -> std::vector<core::Asset> override;

  /**
   * @copydoc IApp::UpdateAssets
   */
  void UpdateAssets(std::vector<core::Asset> assets) override;

  /**
   * @copydoc IApp::SelectSymbolsWithPriceRecords
   */
  auto SelectSymbolsWithPriceRecords [[nodiscard]] () const
      -> std::vector<core::Symbol> override;

  /**
   * @copydoc IApp::SelectSymbolInfo
   */
  auto SelectSymbolInfo [[nodiscard]] (core::Symbol symbol) const
      -> cpp::Opt<core::SymbolInfo> override;

  /**
   * @copydoc IApp::SelectSymbolsInfo
   */
  auto SelectSymbolsInfo [[nodiscard]] () const
      -> std::vector<core::SymbolInfo> override;

  /**
   * @copydoc IApp::UpdateSymbolsInfo
   */
  void UpdateSymbolsInfo(std::vector<core::SymbolInfo> infos) override;

  /**
   * @copydoc IApp::SelectSymbolPriceRecords
   */
  auto SelectSymbolPriceRecords
      [[nodiscard]] (const SelectSymbolPriceRecordsArgs &args) const
      -> std::vector<core::SymbolPriceRecord> override;

  /**
   * @copydoc IApp::InsertSymbolPriceRecord
   */
  void InsertSymbolPriceRecord(core::SymbolPriceRecord record) override;

  /**
   * @copydoc IApp::DeleteSymbolPriceRecords
   */
  void DeleteSymbolPriceRecords(absl::Time before_time) override;

 private:
  network::RestClient rest_client_;
};
}  // namespace stonks::app::a

#endif  // STONKS_APP_ACTION_APP_A_SDB_APP_H_
