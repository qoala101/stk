#include "get_file_service.h"

#include <cpprest/filestream.h>
#include <spdlog/spdlog.h>

namespace stonks {
namespace {
std::string GetFilePathForRequestPath(std::string_view request_path) {
  if (request_path == "/") {
    return "../web/index.html";
  }

  return "../web" + std::string{request_path};
}

std::string_view GetContentTypeForFilePath(std::string_view file_path) {
  if (file_path.ends_with(".html")) {
    return "text/html";
  }

  if (file_path.ends_with(".css")) {
    return "text/css";
  }

  if (file_path.ends_with(".js")) {
    return "text/javascript";
  }

  if (file_path.ends_with(".ico")) {
    return "image/x-icon";
  }

  return "text/plain";
}

void HandleGetRequest(const web::http::http_request &request) {
  const auto request_path = request.absolute_uri().path();
  spdlog::info("Requested URI {}", request_path);

  const auto file_path = GetFilePathForRequestPath(request_path);
  auto open_file_stream_task =
      decltype(concurrency::streams::fstream::open_istream(
          decltype(file_path){})){};

  try {
    open_file_stream_task =
        concurrency::streams::fstream::open_istream(file_path);
  } catch (...) {
    spdlog::info("Cannot read requested file {}", file_path);
    request.reply(web::http::status_codes::NotFound);
    return;
  }

  const auto file_stream = open_file_stream_task.get();
  const auto content_type = GetContentTypeForFilePath(file_path);
  request.reply(web::http::status_codes::OK, file_stream,
                std::string{content_type});
}
}  // namespace

pplx::task<void> GetFileService::Start() {
  http_listener_ = web::http::experimental::listener::http_listener{
      "http://localhost:6506/"};
  http_listener_.support(web::http::methods::GET, HandleGetRequest);
  return http_listener_.open();
}

pplx::task<void> GetFileService::Stop() { return http_listener_.close(); }
}  // namespace stonks