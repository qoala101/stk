#ifndef STONKS_JSON_ANY_TYPES_H_
#define STONKS_JSON_ANY_TYPES_H_

#include <any>

#include "json_converter.h"

namespace stonks::json {
struct Any {
  std::any value{};
  JsonConverterVariant converter{};
} __attribute__((aligned(32)));  // NOLINT(*-magic-numbers)

enum class OptionalFlag { kOptional, kMandatory };

struct AnyDesc {
  json::JsonConverterVariant converter{};
  OptionalFlag optional{};
} __attribute__((aligned(8)));  // NOLINT(*-magic-numbers)
}  // namespace stonks::json

#endif  // STONKS_JSON_ANY_TYPES_H_
