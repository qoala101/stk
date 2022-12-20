#include <fmt/core.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <polymorphic_value.h>

#include <cppcoro/sync_wait.hpp>
#include <map>
#include <memory>
#include <not_null.hpp>
#include <ostream>
#include <string>

#include "core_types.h"
#include "cpp_not_null.h"
#include "cpp_polymorphic_value.h"
#include "gtest/gtest_pred_impl.h"
#include "network_i_json.h"
#include "network_i_rest_request_handler.h"
#include "network_i_rest_request_receiver.h"
#include "network_json_base_conversions.h"
#include "network_json_common_conversions.h"
#include "network_json_conversions_facades.h"
#include "network_rest_request_builder.h"
#include "network_types.h"
#include "restsdk_rest_request_sender.h"
#include "test_restsdk_injector.h"

namespace {
struct SymbolPrice {
  stonks::core::Symbol symbol{};
  double price{};

  //  private:
  //   friend auto operator== [[nodiscard]] (const SymbolPrice &, const
  //   SymbolPrice &)
  //       -> bool = default;
  //   friend auto operator<< [[nodiscard]] (std::ostream &stream,
  //                                        const SymbolPrice &avg_price)
  //       -> std::ostream & {
  //     return stream << avg_price.symbol << " " << avg_price.price;
  //   }
};
}  // namespace

namespace stonks::network {
template <>
auto JsonParser<SymbolPrice>::operator()(const IJson &json) const -> Type {
  return {.symbol = ParseFromJsonChild<core::Symbol>(json, "symbol"),
          .price = std::stod(ParseFromJsonChild<std::string>(json, "price"))};
}

auto ConvertToJson(const SymbolPrice &value) -> cpp::Pv<IJson> {
  // clang-format off
  return BuildJsonFrom(
    "symbol", value.symbol,
    "price", fmt::format("{}", value.price)
  );
  // clang-format on
}
}  // namespace stonks::network

namespace {
TEST(RestRequestReceiver, SendRequest) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    struct Handler : public stonks::network::IRestRequestHandler {
      auto HandleRequestAndGiveResponse
          [[nodiscard]] (stonks::network::RestRequest request) const
          -> cppcoro::task<stonks::network::RestResponse> override {
        EXPECT_EQ(request.endpoint.method, stonks::network::Method::kGet);
        EXPECT_EQ(request.endpoint.uri, stonks::network::Uri{"/Test"});
        co_return stonks::network::RestResponse{
            stonks::network::Status::kOk,
            stonks::network::ConvertToJson(SymbolPrice{
                .symbol = stonks::network::ParseFromJson<stonks::core::Symbol>(
                    *request.body),
                .price = stonks::network::ParseFromJson<double>(
                    *request.params.at("price"))})};
      }
    };

    const auto receiver = []() {
      auto receiver =
          test::restsdk::Injector()
              .create<
                  stonks::cpp::NnUp<stonks::network::IRestRequestReceiver>>();
      receiver->Receive({"http://0.0.0.0:30001"},
                        stonks::cpp::MakeNnUp<Handler>());
      return receiver;
    }();

    const auto request = stonks::network::RestRequestBuilder{}
                             .WithMethod(stonks::network::Method::kGet)
                             .WithBaseUri({"http://0.0.0.0:30001"})
                             .AppendUri({"Test"})
                             .WithBody("BTCUSDT")
                             .AddParam("price", 123.456)
                             .Build();
    const auto sender =
        test::restsdk::Injector().create<stonks::restsdk::RestRequestSender>();
    const auto response = co_await sender.SendRequestAndGetResponse(request);
    const auto response_price =
        stonks::network::ParseFromJson<SymbolPrice>(*response.result);
    EXPECT_EQ(response.status, stonks::network::Status::kOk);
    EXPECT_EQ(response_price.symbol, stonks::core::Symbol{"BTCUSDT"});
    EXPECT_EQ(response_price.price, 123.456);
  }());
}
}  // namespace