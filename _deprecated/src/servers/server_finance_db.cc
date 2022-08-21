#include "server_finance_db.h"

#include <gsl/assert>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "endpoints_finance_db.h"
#include "finance_types.h"
#include "uri.h"

namespace stonks::server {
StonksDb::StonksDb(int port, ccutils::Sp<stonks::StonksDb> entity)
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
            entity_->UpdateAssets(body.Parse<std::vector<std::string>>());
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
                body.Parse<std::vector<finance::SymbolInfo>>());
          }}};
}

auto StonksDb::SelectSymbolPriceTicks() const -> network::Endpoint {
  return {endpoints::FinanceDb::SelectSymbolPriceTicks(),
          network::HasResultTakesParams{[this](network::Params params) {
            return entity_->SelectSymbolPriceTicks(
                params["limit"].Parse<std::optional<int>>(),
                params["period"].Parse<std::optional<finance::Period>>(),
                params["symbols"]
                    .Parse<std::optional<std::vector<finance::SymbolName>>>());
          }}};
}

auto StonksDb::InsertSymbolPriceTick() const -> network::Endpoint {
  return {
      endpoints::FinanceDb::InsertSymbolPriceTick(),
      network::NoResultTakesBody{[this](network::Body body) {
        entity_->InsertSymbolPriceTick(body.Parse<finance::SymbolPriceTick>());
      }}};
}
}  // namespace stonks::server