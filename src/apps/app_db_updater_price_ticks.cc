#include "finance_db_client_server.h"
#include "stonks_db_updater_price_ticks.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  const auto app = stonks::DbUpdaterPriceTicks{
      std::make_shared<stonks::finance::FinanceDbClient>(
          "http://localhost:6506/Db")};
  static_cast<void>(getchar());
}