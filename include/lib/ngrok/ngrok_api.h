#ifndef VH_NGROK_API_H_
#define VH_NGROK_API_H_

#include <cppcoro/task.hpp>

#include "cpp_not_null.h"
#include "network_i_rest_request_sender.h"
#include "ngrok_types.h"

namespace vh::ngrok {
/**
 * @brief Client to Ngrok REST API.
 */
class NgrokApi {
 public:
  explicit NgrokApi(cpp::NnUp<network::IRestRequestSender> request_sender);

  NgrokApi(const NgrokApi &) = delete;
  NgrokApi(NgrokApi &&) noexcept;

  auto operator=(const NgrokApi &) -> NgrokApi & = delete;
  auto operator=(NgrokApi &&) noexcept -> NgrokApi &;

  ~NgrokApi();

  auto tunnels [[nodiscard]] () const -> cppcoro::task<Tunnels>;

 private:
  class Impl;

  cpp::NnUp<Impl> impl_;
};
}  // namespace vh::ngrok

#endif  // VH_NGROK_API_H_
