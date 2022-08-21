#ifndef STONKS_STONKS_STONKS_NGROK_AWS_DB_UPDATER_H_
#define STONKS_STONKS_STONKS_NGROK_AWS_DB_UPDATER_H_

#include "ccutils_not_null.h"

namespace stonks {
class NgrokAwsDbUpdater {
 public:
  explicit NgrokAwsDbUpdater();

  NgrokAwsDbUpdater(const NgrokAwsDbUpdater &) = delete;
  NgrokAwsDbUpdater(NgrokAwsDbUpdater &&) noexcept;

  auto operator=(const NgrokAwsDbUpdater &) -> NgrokAwsDbUpdater & = delete;
  auto operator=(NgrokAwsDbUpdater &&) noexcept -> NgrokAwsDbUpdater &;

  ~NgrokAwsDbUpdater();

 private:
  class Impl;
  ccutils::Up<Impl> impl_{};
};
}  // namespace stonks

#endif  // STONKS_STONKS_STONKS_NGROK_AWS_DB_UPDATER_H_
