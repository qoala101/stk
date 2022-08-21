#include <cstdio>

#include "stonks_ngrok_aws_db_updater.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  const auto app = stonks::NgrokAwsDbUpdater{};
  static_cast<void>(getchar());
}