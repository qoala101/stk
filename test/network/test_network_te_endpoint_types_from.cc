#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>

#include <memory>
#include <type_traits>
#include <utility>

#include "network_json_base_conversions.h"
#include "network_json_common_conversions.h"
#include "network_te_endpoint_types_from.h"

namespace {
struct Type {
  void Params(std::string value, const std::string const_value,
              const std::string &const_ref, const std::string *pointer,
              const stonks::cpp::Opt<std::string> &opt,
              const std::vector<std::string> &vec) const {}

  void BodyValue(std::string body) {}
  void BodyConstValue(int, const std::string body, int, int, int, int) {}
  void BodyConstRef(int, int, const std::string &body, int, int, int) {}
  void BodyPointer(int, int, int, const std::string *body, int, int) {}
  void BodyOpt(int, int, int, int, const stonks::cpp::Opt<std::string> &body,
               int) {}
  void BodyVec(int, int, int, int, int, const std::vector<std::string> &body) {}

  void ResultVoid() {}
  auto ResultValue [[nodiscard]] () { return std::string{}; }
  auto ResultOpt [[nodiscard]] () { return stonks::cpp::Opt<std::string>{}; }
  auto ResultVec [[nodiscard]] () { return std::vector<std::string>{}; }
};

const auto value_json = stonks::network::ConvertToJson("text");
const auto null_json = stonks::network::CreateNullJson();
const auto vec_json =
    stonks::network::ConvertToJson(std::vector<std::string>{"text1", "text2"});

TEST(FunctionTypes, Params) {
  const auto types = stonks::network::te::EndpointTypesFrom<&Type::Params>(
      "value", "const_value", "const_ref", "pointer", "opt", "vec");
  EXPECT_TRUE(types.body.empty());
  EXPECT_TRUE(types.result.empty());

  EXPECT_NO_THROW(types.params.at("value")(*value_json));
  EXPECT_NO_THROW(types.params.at("const_value")(*value_json));
  EXPECT_NO_THROW(types.params.at("const_ref")(*value_json));
  EXPECT_NO_THROW(types.params.at("pointer")(*value_json));
  EXPECT_NO_THROW(types.params.at("opt")(*value_json));
  EXPECT_ANY_THROW(types.params.at("vec")(*value_json));

  EXPECT_ANY_THROW(types.params.at("value")(*null_json));
  EXPECT_ANY_THROW(types.params.at("const_value")(*null_json));
  EXPECT_ANY_THROW(types.params.at("const_ref")(*null_json));
  EXPECT_NO_THROW(types.params.at("pointer")(*null_json));
  EXPECT_NO_THROW(types.params.at("opt")(*null_json));
  EXPECT_NO_THROW(types.params.at("vec")(*null_json));

  EXPECT_ANY_THROW(types.params.at("value")(*vec_json));
  EXPECT_ANY_THROW(types.params.at("const_value")(*vec_json));
  EXPECT_ANY_THROW(types.params.at("const_ref")(*vec_json));
  EXPECT_ANY_THROW(types.params.at("pointer")(*vec_json));
  EXPECT_ANY_THROW(types.params.at("opt")(*vec_json));
  EXPECT_NO_THROW(types.params.at("vec")(*vec_json));
}

TEST(FunctionTypes, Body) {
  auto types = stonks::network::te::EndpointTypesFrom<&Type::BodyValue>(
      stonks::network::te::Body{});
  EXPECT_TRUE(types.params.empty());
  EXPECT_NO_THROW(types.body(*value_json));
  EXPECT_ANY_THROW(types.body(*null_json));
  EXPECT_ANY_THROW(types.body(*vec_json));
  EXPECT_TRUE(types.result.empty());

  types = stonks::network::te::EndpointTypesFrom<&Type::BodyConstValue>(
      "0", stonks::network::te::Body{}, "2", "3", "4", "5");
  EXPECT_EQ(types.params.size(), 5);
  EXPECT_NO_THROW(types.body(*value_json));
  EXPECT_ANY_THROW(types.body(*null_json));
  EXPECT_ANY_THROW(types.body(*vec_json));
  EXPECT_TRUE(types.result.empty());

  types = stonks::network::te::EndpointTypesFrom<&Type::BodyConstRef>(
      "0", "1", stonks::network::te::Body{}, "3", "4", "5");
  EXPECT_EQ(types.params.size(), 5);
  EXPECT_NO_THROW(types.body(*value_json));
  EXPECT_ANY_THROW(types.body(*null_json));
  EXPECT_ANY_THROW(types.body(*vec_json));
  EXPECT_TRUE(types.result.empty());

  types = stonks::network::te::EndpointTypesFrom<&Type::BodyPointer>(
      "0", "1", "2", stonks::network::te::Body{}, "4", "5");
  EXPECT_EQ(types.params.size(), 5);
  EXPECT_NO_THROW(types.body(*value_json));
  EXPECT_NO_THROW(types.body(*null_json));
  EXPECT_ANY_THROW(types.body(*vec_json));
  EXPECT_TRUE(types.result.empty());

  types = stonks::network::te::EndpointTypesFrom<&Type::BodyOpt>(
      "0", "1", "2", "3", stonks::network::te::Body{}, "5");
  EXPECT_EQ(types.params.size(), 5);
  EXPECT_NO_THROW(types.body(*value_json));
  EXPECT_NO_THROW(types.body(*null_json));
  EXPECT_ANY_THROW(types.body(*vec_json));
  EXPECT_TRUE(types.result.empty());

  types = stonks::network::te::EndpointTypesFrom<&Type::BodyVec>(
      "0", "1", "2", "3", "4", stonks::network::te::Body{});
  EXPECT_EQ(types.params.size(), 5);
  EXPECT_ANY_THROW(types.body(*value_json));
  EXPECT_NO_THROW(types.body(*null_json));
  EXPECT_NO_THROW(types.body(*vec_json));
  EXPECT_TRUE(types.result.empty());
}

TEST(FunctionTypes, Result) {
  auto types = stonks::network::te::EndpointTypesFrom<&Type::ResultVoid>();
  EXPECT_TRUE(types.params.empty());
  EXPECT_TRUE(types.body.empty());
  EXPECT_TRUE(types.result.empty());

  types = stonks::network::te::EndpointTypesFrom<&Type::ResultValue>();
  EXPECT_TRUE(types.params.empty());
  EXPECT_TRUE(types.body.empty());
  EXPECT_NO_THROW(types.result(*value_json));
  EXPECT_ANY_THROW(types.result(*null_json));
  EXPECT_ANY_THROW(types.result(*vec_json));

  types = stonks::network::te::EndpointTypesFrom<&Type::ResultOpt>();
  EXPECT_TRUE(types.params.empty());
  EXPECT_TRUE(types.body.empty());
  EXPECT_NO_THROW(types.result(*value_json));
  EXPECT_NO_THROW(types.result(*null_json));
  EXPECT_ANY_THROW(types.result(*vec_json));

  types = stonks::network::te::EndpointTypesFrom<&Type::ResultVec>();
  EXPECT_TRUE(types.params.empty());
  EXPECT_TRUE(types.body.empty());
  EXPECT_ANY_THROW(types.result(*value_json));
  EXPECT_NO_THROW(types.result(*null_json));
  EXPECT_NO_THROW(types.result(*vec_json));
}

TEST(FunctionTypesDeathTest, Errors) {
  EXPECT_DEATH(
      std::ignore = stonks::network::te::EndpointTypesFrom<&Type::Params>(
          "value", "value", "const_ref", "pointer", "opt", "vec"),
      "");
  EXPECT_DEATH(
      std::ignore = stonks::network::te::EndpointTypesFrom<&Type::Params>(
          stonks::network::te::Body{}, stonks::network::te::Body{}, "const_ref",
          "pointer", "opt", "vec"),
      "");
}
}  // namespace