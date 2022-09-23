#ifndef STONKS_APP_DOCKER_TEST_COMMON_APP_DT_TYPES_H_
#define STONKS_APP_DOCKER_TEST_COMMON_APP_DT_TYPES_H_

#include <absl/time/time.h>

#include "cpp_typed_struct.h"

namespace stonks::app::dt {
struct Symbol : public cpp::TypedStruct<std::string> {};

struct Price : public cpp::TypedStruct<double> {};

struct SymbolPriceRecord {
  absl::Time time{};
  Symbol symbol{};
  Price price{};
};
}  // namespace stonks::app::dt

#endif  // STONKS_APP_DOCKER_TEST_COMMON_APP_DT_TYPES_H_
