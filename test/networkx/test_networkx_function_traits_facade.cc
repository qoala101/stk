#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>

#include <cppcoro/task.hpp>
#include <memory>
#include <type_traits>
#include <utility>

#include "network_json_base_conversions.h"
#include "network_json_common_conversions.h"
#include "networkx_common.h"
#include "networkx_endpoint_function_traits_facade.h"
#include "networkx_types.h"

namespace {
struct Type {
  auto Params(std::string value, const std::string const_value,
              const std::string &const_ref, const std::string *pointer,
              const stonks::cpp::Opt<std::string> &opt,
              const std::vector<std::string> &vec) const -> cppcoro::task<> {
    co_return;
  }

  auto BodyValue(std::string body) const -> cppcoro::task<> { co_return; }

  auto BodyConstValue(int, const std::string body, int, int, int, int)
      const -> cppcoro::task<> {
    co_return;
  }

  auto BodyConstRef(int, int, const std::string &body, int, int, int)
      const -> cppcoro::task<> {
    co_return;
  }

  auto BodyPointer(int, int, int, const std::string *body, int, int)
      const -> cppcoro::task<> {
    co_return;
  }

  auto BodyOpt(int, int, int, int, const stonks::cpp::Opt<std::string> &body,
               int) const -> cppcoro::task<> {
    co_return;
  }

  auto BodyVec(int, int, int, int, int, const std::vector<std::string> &body)
      const -> cppcoro::task<> {
    co_return;
  }

  auto ResultVoid() const -> cppcoro::task<> { co_return; }

  auto ResultValue [[nodiscard]] () const -> cppcoro::task<std::string> {
    co_return std::string{};
  }

  auto ResultOpt [[nodiscard]] () const 
  -> cppcoro::task<stonks::cpp::Opt<std::string>> {
    co_return stonks::cpp::Opt<std::string>{};
  }

  auto ResultVec [[nodiscard]] () const -> cppcoro::task<std::vector<std::string>> {
    co_return std::vector<std::string>{};
  }

  auto Death1(std::string value, const std::string const_value,
              const std::string &const_ref, const std::string *pointer,
              const stonks::cpp::Opt<std::string> &opt,
              const std::vector<std::string> &vec) const -> cppcoro::task<> {
    co_return;
  }
  auto Death2(std::string value, const std::string const_value,
              const std::string &const_ref, const std::string *pointer,
              const stonks::cpp::Opt<std::string> &opt,
              const std::vector<std::string> &vec) const -> cppcoro::task<> {
    co_return;
  }
};
}  // namespace

namespace stonks::networkx {
template <>
struct EndpointFunctionTraits<&Type::Params> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams =
      ParamList("value", "const_value", "const_ref", "pointer", "opt", "vec");
};

template <>
struct EndpointFunctionTraits<&Type::BodyValue> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams = ParamList(RequestBody{});
};

template <>
struct EndpointFunctionTraits<&Type::BodyConstValue> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams =
      ParamList("0", RequestBody{}, "2", "3", "4", "5");
};

template <>
struct EndpointFunctionTraits<&Type::BodyConstRef> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams =
      ParamList("0", "1", RequestBody{}, "3", "4", "5");
};

template <>
struct EndpointFunctionTraits<&Type::BodyPointer> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams =
      ParamList("0", "1", "2", RequestBody{}, "4", "5");
};

template <>
struct EndpointFunctionTraits<&Type::BodyOpt> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams =
      ParamList("0", "1", "2", "3", RequestBody{}, "5");
};

template <>
struct EndpointFunctionTraits<&Type::BodyVec> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams =
      ParamList("0", "1", "2", "3", "4", RequestBody{});
};

template <>
struct EndpointFunctionTraits<&Type::ResultVoid> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct EndpointFunctionTraits<&Type::ResultValue> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct EndpointFunctionTraits<&Type::ResultOpt> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct EndpointFunctionTraits<&Type::ResultVec> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct EndpointFunctionTraits<&Type::Death1> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams =
      ParamList("value", "value", "const_ref", "pointer", "opt", "vec");
};

template <>
struct EndpointFunctionTraits<&Type::Death2> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams = ParamList(
      RequestBody{}, RequestBody{}, "const_ref", "pointer", "opt", "vec");
};
}  // namespace stonks::networkx

namespace {
const auto value_json = stonks::network::ConvertToJson("text");
const auto null_json = stonks::network::CreateNullJson();
const auto vec_json =
    stonks::network::ConvertToJson(std::vector<std::string>{"text1", "text2"});

TEST(FunctionTypes, Params) {
  const auto types = stonks::networkx::EndpointFunctionTraitsFacade<
                         &Type::Params>::AsTypedEndpoint()
                         .expected_types;
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
  auto types = stonks::networkx::EndpointFunctionTraitsFacade<
                   &Type::BodyValue>::AsTypedEndpoint()
                   .expected_types;
  EXPECT_TRUE(types.params.empty());
  EXPECT_NO_THROW(types.body(*value_json));
  EXPECT_ANY_THROW(types.body(*null_json));
  EXPECT_ANY_THROW(types.body(*vec_json));
  EXPECT_TRUE(types.result.empty());

  types = stonks::networkx::EndpointFunctionTraitsFacade<
              &Type::BodyConstValue>::AsTypedEndpoint()
              .expected_types;
  EXPECT_EQ(types.params.size(), 5);
  EXPECT_NO_THROW(types.body(*value_json));
  EXPECT_ANY_THROW(types.body(*null_json));
  EXPECT_ANY_THROW(types.body(*vec_json));
  EXPECT_TRUE(types.result.empty());

  types = stonks::networkx::EndpointFunctionTraitsFacade<
              &Type::BodyConstRef>::AsTypedEndpoint()
              .expected_types;
  EXPECT_EQ(types.params.size(), 5);
  EXPECT_NO_THROW(types.body(*value_json));
  EXPECT_ANY_THROW(types.body(*null_json));
  EXPECT_ANY_THROW(types.body(*vec_json));
  EXPECT_TRUE(types.result.empty());

  types = stonks::networkx::EndpointFunctionTraitsFacade<
              &Type::BodyPointer>::AsTypedEndpoint()
              .expected_types;
  EXPECT_EQ(types.params.size(), 5);
  EXPECT_NO_THROW(types.body(*value_json));
  EXPECT_NO_THROW(types.body(*null_json));
  EXPECT_ANY_THROW(types.body(*vec_json));
  EXPECT_TRUE(types.result.empty());

  types = stonks::networkx::EndpointFunctionTraitsFacade<
              &Type::BodyOpt>::AsTypedEndpoint()
              .expected_types;
  EXPECT_EQ(types.params.size(), 5);
  EXPECT_NO_THROW(types.body(*value_json));
  EXPECT_NO_THROW(types.body(*null_json));
  EXPECT_ANY_THROW(types.body(*vec_json));
  EXPECT_TRUE(types.result.empty());

  types = stonks::networkx::EndpointFunctionTraitsFacade<
              &Type::BodyVec>::AsTypedEndpoint()
              .expected_types;
  EXPECT_EQ(types.params.size(), 5);
  EXPECT_ANY_THROW(types.body(*value_json));
  EXPECT_NO_THROW(types.body(*null_json));
  EXPECT_NO_THROW(types.body(*vec_json));
  EXPECT_TRUE(types.result.empty());
}

TEST(FunctionTypes, Result) {
  auto types = stonks::networkx::EndpointFunctionTraitsFacade<
                   &Type::ResultVoid>::AsTypedEndpoint()
                   .expected_types;
  EXPECT_TRUE(types.params.empty());
  EXPECT_TRUE(types.body.empty());
  EXPECT_TRUE(types.result.empty());

  types = stonks::networkx::EndpointFunctionTraitsFacade<
              &Type::ResultValue>::AsTypedEndpoint()
              .expected_types;
  EXPECT_TRUE(types.params.empty());
  EXPECT_TRUE(types.body.empty());
  EXPECT_NO_THROW(types.result(*value_json));
  EXPECT_ANY_THROW(types.result(*null_json));
  EXPECT_ANY_THROW(types.result(*vec_json));

  types = stonks::networkx::EndpointFunctionTraitsFacade<
              &Type::ResultOpt>::AsTypedEndpoint()
              .expected_types;
  EXPECT_TRUE(types.params.empty());
  EXPECT_TRUE(types.body.empty());
  EXPECT_NO_THROW(types.result(*value_json));
  EXPECT_NO_THROW(types.result(*null_json));
  EXPECT_ANY_THROW(types.result(*vec_json));

  types = stonks::networkx::EndpointFunctionTraitsFacade<
              &Type::ResultVec>::AsTypedEndpoint()
              .expected_types;
  EXPECT_TRUE(types.params.empty());
  EXPECT_TRUE(types.body.empty());
  EXPECT_ANY_THROW(types.result(*value_json));
  EXPECT_NO_THROW(types.result(*null_json));
  EXPECT_NO_THROW(types.result(*vec_json));
}

TEST(FunctionTypesDeathTest, Errors) {
  EXPECT_DEATH(std::ignore = stonks::networkx::EndpointFunctionTraitsFacade<
                                 &Type::Death1>::AsTypedEndpoint()
                                 .expected_types,
               "");
  EXPECT_DEATH(std::ignore = stonks::networkx::EndpointFunctionTraitsFacade<
                                 &Type::Death2>::AsTypedEndpoint()
                                 .expected_types,
               "");
}
}  // namespace