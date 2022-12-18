#ifndef STONKS_NGROK_NGROK_API_H_
#define STONKS_NGROK_NGROK_API_H_

#include <cppcoro/task.hpp>
#include <string>

#include "cpp_not_null.h"
#include "di_factory.h"
#include "network_i_rest_request_sender.h"
#include "ngrok_types.h"

namespace stonks::ngrok {
/**
 * @brief Client to Ngrok REST API.
 */
class NgrokApi {
 public:
  /**
   * @param host Host which runs Ngrok.
   */
  NgrokApi(std::string_view host,
           di::Factory<network::IRestRequestSender> request_sender_factory);

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
}  // namespace stonks::ngrok

#endif  // STONKS_NGROK_NGROK_API_H_
