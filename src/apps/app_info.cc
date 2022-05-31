#include "info_server.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  const auto app = stonks::InfoServer{"http://localhost:6506/Db"};
  static_cast<void>(getchar());
}