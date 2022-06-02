#include "server_finance_db.h"

#include <gsl/assert>

#include "endpoints_finance_db.h"
#include "uri.h"

namespace stonks {
StonksDbServer::StonksDbServer(int port, std::shared_ptr<StonksDb> entity)
    : server_{network::LocalUri{port, kEndpoint},
              {SelectAssets(), UpdateAssets(), SelectSymbols(),
               SelectSymbolsInfo(), UpdateSymbolsInfo(),
               SelectSymbolPriceTicks(), InsertSymbolPriceTick()}},
      entity_{std::move(entity)} {
  Expects(entity_ != nullptr);
}

auto StonksDbServer::SelectAssets() const -> network::Endpoint {
  return {FinanceDbEndpoints::SelectAssets(),
          network::HasResult{[this]() { return entity_->SelectAssets(); }}};
}

auto StonksDbServer::UpdateAssets() const -> network::Endpoint {
  return {FinanceDbEndpoints::UpdateAssets(),
          network::NoResultTakesBody{[this](network::Body body) {
            entity_->UpdateAssets(body.Get<std::vector<std::string>>());
          }}};
}

auto StonksDbServer::SelectSymbols() const -> network::Endpoint {
  return {FinanceDbEndpoints::SelectSymbols(),
          network::HasResult{[this]() { return entity_->SelectSymbols(); }}};
}

auto StonksDbServer::SelectSymbolsInfo() const -> network::Endpoint {
  return {FinanceDbEndpoints::SelectSymbolsInfo(), network::HasResult{[this]() {
            return entity_->SelectSymbolsInfo();
          }}};
}

auto StonksDbServer::UpdateSymbolsInfo() const -> network::Endpoint {
  return {FinanceDbEndpoints::UpdateSymbolsInfo(),
          network::NoResultTakesBody{[this](network::Body body) {
            entity_->UpdateSymbolsInfo(
                body.Get<std::vector<finance::SymbolInfo>>());
          }}};
}

auto StonksDbServer::SelectSymbolPriceTicks() const -> network::Endpoint {
  return {FinanceDbEndpoints::SelectSymbolPriceTicks(),
          network::HasResultTakesParams{[this](network::Params params) {
            return entity_->SelectSymbolPriceTicks(
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
        entity_->InsertSymbolPriceTick(body.Get<finance::SymbolPriceTick>());
      }}};
}
}  // namespace stonks