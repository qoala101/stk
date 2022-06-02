#include "server_finance_db.h"

#include <gsl/assert>

#include "endpoints_finance_db.h"
#include "uri.h"

namespace stonks::server {
StonksDb::StonksDb(int port, std::shared_ptr<stonks::StonksDb> entity)
    : server_{network::LocalUri{port, kEndpoint},
              {SelectAssets(), UpdateAssets(), SelectSymbols(),
               SelectSymbolsInfo(), UpdateSymbolsInfo(),
               SelectSymbolPriceTicks(), InsertSymbolPriceTick()}},
      entity_{std::move(entity)} {
  Expects(entity_ != nullptr);
}

auto StonksDb::SelectAssets() const -> network::Endpoint {
  return {endpoints::FinanceDb::SelectAssets(),
          network::HasResult{[this]() { return entity_->SelectAssets(); }}};
}

auto StonksDb::UpdateAssets() const -> network::Endpoint {
  return {endpoints::FinanceDb::UpdateAssets(),
          network::NoResultTakesBody{[this](network::Body body) {
            entity_->UpdateAssets(body.Get<std::vector<std::string>>());
          }}};
}

auto StonksDb::SelectSymbols() const -> network::Endpoint {
  return {endpoints::FinanceDb::SelectSymbols(),
          network::HasResult{[this]() { return entity_->SelectSymbols(); }}};
}

auto StonksDb::SelectSymbolsInfo() const -> network::Endpoint {
  return {
      endpoints::FinanceDb::SelectSymbolsInfo(),
      network::HasResult{[this]() { return entity_->SelectSymbolsInfo(); }}};
}

auto StonksDb::UpdateSymbolsInfo() const -> network::Endpoint {
  return {endpoints::FinanceDb::UpdateSymbolsInfo(),
          network::NoResultTakesBody{[this](network::Body body) {
            entity_->UpdateSymbolsInfo(
                body.Get<std::vector<finance::SymbolInfo>>());
          }}};
}

auto StonksDb::SelectSymbolPriceTicks() const -> network::Endpoint {
  return {endpoints::FinanceDb::SelectSymbolPriceTicks(),
          network::HasResultTakesParams{[this](network::Params params) {
            return entity_->SelectSymbolPriceTicks(
                params.Get<std::optional<int>>("limit"),
                params.Get<std::optional<finance::Period>>("period"),
                params.Get<std::optional<std::vector<finance::SymbolName>>>(
                    "symbols"));
          }}};
}

auto StonksDb::InsertSymbolPriceTick() const -> network::Endpoint {
  return {
      endpoints::FinanceDb::InsertSymbolPriceTick(),
      network::NoResultTakesBody{[this](network::Body body) {
        entity_->InsertSymbolPriceTick(body.Get<finance::SymbolPriceTick>());
      }}};
}
}  // namespace stonks::server