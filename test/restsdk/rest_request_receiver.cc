#include <gtest/gtest.h>
#include <polymorphic_value.h>

#include <cstdint>
#include <magic_enum.hpp>
#include <ostream>
#include <string>
#include <thread>

#include "cpp_not_null.h"
#include "network_enums.h"
#include "network_i_json.h"
#include "network_json_basic_conversions.h"
#include "network_rest_request_builder.h"
#include "network_types.h"
#include "restsdk_factory.h"
#include "restsdk_rest_request_receiver.h"
#include "restsdk_rest_request_sender.h"
#include "stonks_types.h"

namespace {
struct SymbolPrice {
  stonks::SymbolName symbol{};
  double price{};

 private:
  friend auto operator==(const SymbolPrice &, const SymbolPrice &)
      -> bool = default;
  friend auto operator<<(std::ostream &stream, const SymbolPrice &avg_price)
      -> std::ostream & {
    return stream << avg_price.symbol << " " << avg_price.price;
  }
};
}  // namespace

namespace stonks::network {
template <>
auto ParseFromJson(const IJson &json) -> SymbolPrice {
  return SymbolPrice{
      ParseFromJson<SymbolName>(*json.GetChild("symbol")),
      std::stod(ParseFromJson<std::string>(*json.GetChild("price")))};
}

auto ConvertToJson(const SymbolPrice &value) -> cpp::Pv<IJson> {
  auto json = CreateNullJson();
  json->SetChild("symbol", ConvertToJson(value.symbol));
  json->SetChild("price", ConvertToJson(std::to_string(value.price)));
  return json;
}
}  // namespace stonks::network

namespace {
TEST(RestRequestReceiver, SendRequest) {
  struct Handler : public stonks::network::IRestRequestHandler {
    [[nodiscard]] auto HandleRequestAndGiveResponse(
        stonks::network::RestRequest request) const
        -> stonks::network::RestResponse override {
      EXPECT_EQ(request.endpoint.method, stonks::network::Method::kGet);
      EXPECT_EQ(request.endpoint.uri, "/Test");
      return {stonks::network::Status::kOk,
              stonks::network::ConvertToJson(SymbolPrice{
                  .symbol = stonks::network::ParseFromJson<stonks::SymbolName>(
                      **request.body),
                  .price = stonks::network::ParseFromJson<double>(
                      *request.params.at("price"))})};
    }
  };

  const auto receiver = stonks::restsdk::RestRequestReceiver{
      "http://localhost:6506", stonks::cpp::MakeNnUp<Handler>()};

  const auto request = stonks::network::RestRequestBuilder{}
                           .WithMethod(stonks::network::Method::kGet)
                           .WithBaseUri("http://localhost:6506")
                           .AppendUri("Test")
                           .WithBody(stonks::network::ConvertToJson("BTCUSDT"))
                           .AddParam("price", 123.456)
                           .Build();
  const auto sender = stonks::restsdk::RestRequestSender{};
  const auto response = sender.SendRequestAndGetResponse(request);
  const auto response_price = ParseFromJson<SymbolPrice>(**response.result);
  EXPECT_EQ(response.status, stonks::network::Status::kOk);
  EXPECT_EQ(response_price.symbol, "BTCUSDT");
  EXPECT_EQ(response_price.price, 123.456);
}
}  // namespace