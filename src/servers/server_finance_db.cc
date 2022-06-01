#include "server_finance_db.h"

#include <gsl/assert>

#include "endpoints_finance_db.h"

namespace stonks {
StonksDbServer::StonksDbServer(std::string_view base_uri,
                               std::shared_ptr<StonksDb> stonks_db)
    : server_{base_uri,
              {SelectAssets(), UpdateAssets(), SelectSymbols(),
               SelectSymbolsInfo(), UpdateSymbolsInfo(),
               SelectSymbolPriceTicks(), InsertSymbolPriceTick()}},
      stonks_db_{std::move(stonks_db)} {
  Expects(stonks_db_ != nullptr);
}

auto StonksDbServer::SelectAssets() const -> network::Endpoint {
  return {FinanceDbEndpoints::SelectAssets(),
          network::HasResult{[this]() { return stonks_db_->SelectAssets(); }}};
}

auto StonksDbServer::UpdateAssets() const -> network::Endpoint {
  return {FinanceDbEndpoints::UpdateAssets(),
          network::NoResultTakesBody{[this](network::Body body) {
            stonks_db_->UpdateAssets(body.Get<std::vector<std::string>>());
          }}};
}

auto StonksDbServer::SelectSymbols() const -> network::Endpoint {
  return {FinanceDbEndpoints::SelectSymbols(),
          network::HasResult{[this]() { return stonks_db_->SelectSymbols(); }}};
}

auto StonksDbServer::SelectSymbolsInfo() const -> network::Endpoint {
  return {FinanceDbEndpoints::SelectSymbolsInfo(), network::HasResult{[this]() {
            return stonks_db_->SelectSymbolsInfo();
          }}};
}

auto StonksDbServer::UpdateSymbolsInfo() const -> network::Endpoint {
  return {FinanceDbEndpoints::UpdateSymbolsInfo(),
          network::NoResultTakesBody{[this](network::Body body) {
            stonks_db_->UpdateSymbolsInfo(
                body.Get<std::vector<finance::SymbolInfo>>());
          }}};
}

auto StonksDbServer::SelectSymbolPriceTicks() const -> network::Endpoint {
  return {FinanceDbEndpoints::SelectSymbolPriceTicks(),
          network::HasResultTakesParams{[this](network::Params params) {
            return stonks_db_->SelectSymbolPriceTicks(
                params.Get<std::optional<int>>("limit"),
                params.Get<std::optional<finance::Period>>("period"),
                params.Get<std::optional<std::vector<finance::SymbolName>>>(
                    "symbols"));
          }}};
}

auto StonksDbServer::InsertSymbolPriceTick() const -> network::Endpoint {
  return {
      FinanceDbEndpoints::InsertSymbolPriceTick(),
      network::NoResultTakesBody{[this](network::Body body) {
        stonks_db_->InsertSymbolPriceTick(body.Get<finance::SymbolPriceTick>());
      }}};
}
}  // namespace stonks