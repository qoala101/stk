/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "restsdk_parse_json_fom_string.h"

#include <cpprest/json.h>

#include <memory>
#include <system_error>
#include <utility>

#include "restsdk_json.h"
#include "restsdk_json_native_handle.h"

namespace vh::restsdk {
auto ParseJsonFromString(const std::string &value) -> cpp::Pv<network::IJson> {
  auto error_code = std::error_code{};
  auto json = web::json::value::parse(value, error_code);

  if (const auto parsing_failed = error_code.value() > 0) {
    json = web::json::value::string(value);
  }

  return cpp::MakePv<network::IJson, Json>(
      network::IJson::NativeHandle{std::move(json)});
}
}  // namespace vh::restsdk