#ifndef STONKS_APP_SERVICE_SERVICE_CLIENT_BASE_H_
#define STONKS_APP_SERVICE_SERVICE_CLIENT_BASE_H_

#include "networkx_client.h"
#include "networkx_concepts.h"

namespace stonks::service {
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
  template <cpp::MemberFunctionOf<Target> auto kFunction, typename... Args,
            typename ResultType = typename networkx::
                EndpointFunctionTraitsFacade<kFunction>::ResultType>
  auto Call [[nodiscard]] (Args &&...args) const -> cppcoro::task<ResultType> {
    co_return co_await client_.template Call<kFunction>(
        std::forward<Args>(args)...);
  }

 private:
  networkx::Client<Target> client_;
};
}  // namespace stonks::service

#endif  // STONKS_APP_SERVICE_SERVICE_CLIENT_BASE_H_
