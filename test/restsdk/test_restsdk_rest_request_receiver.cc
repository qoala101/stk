#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <polymorphic_value.h>

#include <boost/di.hpp>
#include <map>
#include <memory>
#include <ostream>
#include <string>

#include "cpp_format.h"
#include "cpp_not_null.h"
#include "cpp_polymorphic_value.h"
#include "gtest/gtest_pred_impl.h"
#include "network_enums.h"
#include "network_i_json.h"
#include "network_i_rest_request_handler.h"
#include "network_i_rest_request_receiver.h"
#include "network_json_basic_conversions.h"
#include "network_json_common_conversions.h"
#include "network_json_conversions_facades.h"
#include "network_rest_request_builder.h"
#include "network_types.h"
#include "not_null.hpp"
#include "restsdk_rest_request_sender.h"
#include "stonks_types.h"
#include "test_restsdk_injector.h"

namespace {
struct SymbolPrice {
  stonks::SymbolName symbol{};
  double price{};

 private:
  [[nodiscard]] friend auto operator==(const SymbolPrice &, const SymbolPrice &)
      -> bool = default;
  [[nodiscard]] friend auto operator<<(std::ostream &stream,
                                       const SymbolPrice &avg_price)
      -> std::ostream & {
    return stream << avg_price.symbol << " " << avg_price.price;
  }
};
}  // namespace

namespace stonks::network {
template <>
auto ParseFromJson(const IJson &json) -> SymbolPrice {
  return SymbolPrice{ParseFromJsonChild<SymbolName>(json, "symbol"),
                     std::stod(ParseFromJsonChild<std::string>(json, "price"))};
}

auto ConvertToJson(const SymbolPrice &value) -> cpp::Pv<IJson> {
  // clang-format off
  return BuildJsonFrom(
    "symbol", value.symbol,
    "price", cpp::Format("{}", value.price)
  );
  // clang-format on
}
}  // namespace stonks::network

namespace {
TEST(RestRequestReceiver, SendRequest) {
  struct Handler : public stonks::network::IRestRequestHandler {
    [[nodiscard]] auto HandleRequestAndGiveResponse(
        stonks::network::RestRequest request) const
        -> stonks::network::RestResponse override {
      EXPECT_EQ(request.endpoint.method, stonks::network::Method::kGet);
      EXPECT_EQ(request.endpoint.uri, stonks::network::Uri{"/Test"});
      return {stonks::network::Status::kOk,
              stonks::network::ConvertToJson(SymbolPrice{
                  .symbol = stonks::network::ParseFromJson<stonks::SymbolName>(
                      **request.body),
                  .price = stonks::network::ParseFromJson<double>(
                      *request.params.at("price"))})};
    }
  };

  const auto receiver = []() {
    auto receiver =
        test::restsdk::Injector()
            .create<stonks::cpp::NnUp<stonks::network::IRestRequestReceiver>>();
    receiver->Receive({"http://localhost:6506"},
                      stonks::cpp::MakeNnSp<Handler>());
    return receiver;
  }();

  const auto request = stonks::network::RestRequestBuilder{}
                           .WithMethod(stonks::network::Method::kGet)
                           .WithBaseUri({"http://localhost:6506"})
                           .AppendUri({"Test"})
                           .WithBody("BTCUSDT")
                           .AddParam("price", 123.456)
                           .Build();
  const auto sender =
      test::restsdk::Injector().create<stonks::restsdk::RestRequestSender>();
  const auto response = sender.SendRequestAndGetResponse(request);
  const auto response_price = ParseFromJson<SymbolPrice>(**response.result);
  EXPECT_EQ(response.status, stonks::network::Status::kOk);
  EXPECT_EQ(response_price.symbol, stonks::SymbolName{"BTCUSDT"});
  EXPECT_EQ(response_price.price, 123.456);
}
}  // namespace