#ifndef STONKS_AWS_DYNAMODB_AWS_DYNAMODB_CALL_AS_COROUTINE_H_
#define STONKS_AWS_DYNAMODB_AWS_DYNAMODB_CALL_AS_COROUTINE_H_

#include <callable.hpp>
#include <cppcoro/single_consumer_event.hpp>
#include <cppcoro/task.hpp>
#include <member_function.hpp>

#include "cpp_concepts.h"

namespace Aws::DynamoDB {
class DynamoDBClient;  // IWYU pragma: keep
}  // namespace Aws::DynamoDB

namespace stonks::aws::dynamodb {
/**
 * @brief Util which allows to call member async functions as coroutines.
 */
template <
    cpp::MemberFunctionOf<Aws::DynamoDB::DynamoDBClient> auto kFunction,
    typename FunctionType = decltype(kFunction),
    typename NativeTraits = member_function_traits<FunctionType>,
    typename RequestType = typename NativeTraits::template argument_type<0>,
    typename HandlerType = typename NativeTraits::template argument_type<1>,
    typename OutcomeType = std::remove_cvref_t<
        typename callable_traits<HandlerType>::template argument_type<2>>>
auto CallAsCoroutine
    [[nodiscard]] (const Aws::DynamoDB::DynamoDBClient &db_client,
                   const RequestType &request) -> cppcoro::task<OutcomeType> {
  auto result = OutcomeType{};
  auto result_is_ready = cppcoro::single_consumer_event{};

  (db_client.*kFunction)(
      request,
      [&result, &result_is_ready](const auto &, const auto &,
                                  const auto &outcome, const auto &) mutable {
        result = outcome;
        result_is_ready.set();
      },
      nullptr);

  co_await result_is_ready;
  co_return result;
}
}  // namespace stonks::aws::dynamodb

#endif  // STONKS_AWS_DYNAMODB_AWS_DYNAMODB_CALL_AS_COROUTINE_H_
