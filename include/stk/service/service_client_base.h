/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_STK_SERVICE_CLIENT_BASE_H_
#define VH_STK_SERVICE_CLIENT_BASE_H_

#include "networkx_client.h"
#include "networkx_concepts.h"

namespace vh::stk::service {
/**
 * @brief Convenience base for service clients.
 */
template <networkx::ClientServerType TargetT>
class ClientBase : public TargetT {
 public:
  explicit ClientBase(networkx::Client<TargetT> client)
      : client_{std::move(client)} {}

 protected:
  using Target = TargetT;

  /**
   * @copydoc networkx::Client::Call
   */
  template <auto kFunction, typename... Args,
            typename ResultType = typename networkx::
                EndpointFunctionTraitsFacade<kFunction>::ResultType>
    requires cpp::MemberFunctionOf<decltype(kFunction), Target>
  auto Call [[nodiscard]] (Args &&...args) const -> cppcoro::task<ResultType> {
    co_return co_await client_.template Call<kFunction>(
        std::forward<Args>(args)...);
  }

 private:
  networkx::Client<Target> client_;
};
}  // namespace vh::stk::service

#endif  // VH_STK_SERVICE_CLIENT_BASE_H_
