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
  vh::stk::core::Symbol symbol{};
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

namespace vh::network {
template <>
auto JsonParser<SymbolPrice>::operator()(const IJson &json) const -> Type {
  return {.symbol = ParseFromJsonChild<stk::core::Symbol>(json, "symbol"),
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
}  // namespace vh::network

namespace {
TEST(RestRequestReceiver, SendRequest) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    struct Handler : public vh::network::IRestRequestHandler {
      auto HandleRequestAndGiveResponse
          [[nodiscard]] (vh::network::RestRequest request)
          -> cppcoro::task<vh::network::RestResponse> override {
        EXPECT_EQ(request.endpoint.method, vh::network::Method::kGet);
        EXPECT_EQ(request.endpoint.uri, vh::network::Uri{"/Test"});
        co_return vh::network::RestResponse{
            vh::network::Status::kOk,
            vh::network::ConvertToJson(SymbolPrice{
                .symbol = vh::network::ParseFromJson<vh::stk::core::Symbol>(
                    *request.body),
                .price = vh::network::ParseFromJson<double>(
                    *request.params.at("price"))})};
      }
    };

    const auto receiver = []() {
      auto receiver =
          test::restsdk::Injector()
              .create<
                  vh::cpp::NnUp<vh::network::IRestRequestReceiver>>();
      receiver->Receive({"http://0.0.0.0:30001"},
                        vh::cpp::MakeNnUp<Handler>());
      return receiver;
    }();

    const auto request = vh::network::RestRequestBuilder{}
                             .WithMethod(vh::network::Method::kGet)
                             .WithBaseUri({"http://0.0.0.0:30001"})
                             .AppendUri({"Test"})
                             .WithBody("BTCUSDT")
                             .AddParam("price", 123.456)
                             .Build();
    auto sender =
        test::restsdk::Injector().create<vh::restsdk::RestRequestSender>();
    const auto response = co_await sender.SendRequestAndGetResponse(request);
    const auto response_price =
        vh::network::ParseFromJson<SymbolPrice>(*response.result);
    EXPECT_EQ(response.status, vh::network::Status::kOk);
    EXPECT_EQ(response_price.symbol, vh::stk::core::Symbol{"BTCUSDT"});
    EXPECT_EQ(response_price.price, 123.456);
  }());
}
}  // namespace