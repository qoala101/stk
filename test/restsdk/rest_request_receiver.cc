#include <gtest/gtest.h>
#include <polymorphic_value.h>

#include <cstdint>
#include <magic_enum.hpp>
#include "cpp_not_null.h"
#include <ostream>
#include <string>
#include <thread>

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

auto ConvertToJson(const SymbolPrice &value)
    -> cpp::Pv<IJson> {
  auto json = restsdk::Factory{}.CreateJson();
  json->SetChild("symbol", *ConvertToJson(value.symbol));
  json->SetChild("price", *ConvertToJson(std::to_string(value.price)));
  return json;
}
}  // namespace stonks::network

namespace {
TEST(RestRequestReceiver, SendRequest) {
  // auto receiver_thread = std::jthread([]() {
  const auto receiver = cpp::MakeUp<stonks::restsdk::RestRequestReceiver>(
      "http://localhost:6506",
      [](stonks::network::Endpoint endpoint,
         stonks::network::RestRequestData data)
          -> std::pair<stonks::network::Status, stonks::network::Result> {
        EXPECT_EQ(endpoint.method, stonks::network::Method::kGet);
        EXPECT_EQ(endpoint.uri, "/Test");
        return {
            stonks::network::Status::kOk,
            stonks::network::ConvertToJson(SymbolPrice{
                .symbol = stonks::network::ParseFromJson<stonks::SymbolName>(
                    **data.body),
                .price = stonks::network::ParseFromJson<double>(
                    *data.params.at("price"))})};
      });
  // });

  const auto [endpoint, data] =
      stonks::network::RestRequestBuilder{}
          .WithMethod(stonks::network::Method::kGet)
          .WithBaseUri("http://localhost:6506")
          .AppendUri("Test")
          .WithBody(stonks::network::ConvertToJson("BTCUSDT"))
          .AddParam("price", 123.456)
          .Build();
  const auto sender =
      cpp::MakeUp<stonks::restsdk::RestRequestSender>(endpoint);
  const auto response = sender->SendRequestAndGetResponse(data);
  const auto response_price = ParseFromJson<SymbolPrice>(*response.second);
  EXPECT_EQ(response.first, stonks::network::Status::kOk);
  EXPECT_EQ(response_price.symbol, "BTCUSDT");
  EXPECT_EQ(response_price.price, 123.456);
}
}  // namespace