#ifndef STONKS_APP_SERVICE_SERVICE_CLIENT_H_
#define STONKS_APP_SERVICE_SERVICE_CLIENT_H_

#include "networkx_client.h"
#include "networkx_concepts.h"

namespace stonks::service {
/**
 * @brief Base for service clients.
 */
template <networkx::ClientServerType TargetT>
class ServiceClientBase : public TargetT {
 public:
  explicit ServiceClientBase(networkx::Client<TargetT> client)
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

/**
 * @brief Client to service.
 * Should extend service client base and implement all target virtual functions
 * using provided Call.
 */
template <networkx::ClientServerType Target>
class ServiceClient;
}  // namespace stonks::service

#endif  // STONKS_APP_SERVICE_SERVICE_CLIENT_H_
