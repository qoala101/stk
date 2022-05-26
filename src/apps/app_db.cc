#include "finance_db.h"
#include "finance_db_client_server.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  const auto app = stonks::finance::StonksDbServer{
      "http://localhost:6506/Db",
      std::make_shared<stonks::finance::FinanceDb>(":memory:")};
  static_cast<void>(getchar());
}