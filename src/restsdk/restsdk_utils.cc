#include "restsdk_utils.h"

namespace stonks::restsdk::utils {
auto MethodFromNetworkMethod(network::Method method) -> web::http::method {
  switch (method) {
    case network::Method::kGet:
      return web::http::methods::GET;
    case network::Method::kPost:
      return web::http::methods::POST;
    case network::Method::kPut:
      return web::http::methods::PUT;
    case network::Method::kDelete:
      return web::http::methods::DEL;
    case network::Method::kHead:
      return web::http::methods::HEAD;
    case network::Method::kOptions:
      return web::http::methods::OPTIONS;
    case network::Method::kTrace:
      return web::http::methods::TRCE;
    case network::Method::kConnect:
      return web::http::methods::CONNECT;
    case network::Method::kMerge:
      return web::http::methods::MERGE;
    case network::Method::kPatch:
      return web::http::methods::PATCH;
  }
}
}  // namespace stonks::restsdk::utils