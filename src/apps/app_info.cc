#include "info_server.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  const auto app = stonks::InfoServer{"http://localhost:6507/Info"};
  static_cast<void>(getchar());
}