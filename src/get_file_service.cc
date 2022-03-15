#include "get_file_service.h"

#include <cpprest/filestream.h>
#include <spdlog/spdlog.h>

namespace
{
  web::uri BaseUri()
  {
    auto uri_builder = web::uri_builder{};
    uri_builder.set_scheme("http");
    uri_builder.set_host("localhost");
    uri_builder.set_port(6506);
    return uri_builder.to_uri();
  }

  std::string ContentTypeFromFilePath(const std::string &file_path)
  {
    if (file_path.ends_with(".html"))
    {
      return "text/html";
    }

    if (file_path.ends_with(".css"))
    {
      return "text/css";
    }

    if (file_path.ends_with(".js"))
    {
      return "text/js";
    }

    return "text/plain";
  }

  void HandleGetRequest(const stonks::GetFileService &service, const web::http::http_request &request)
  {
    const auto request_path = request.absolute_uri().path();
    spdlog::info("GET: requested URI {}", request_path);

    const auto &uri_to_file_path_mapping = service.UriToFilePathMapping();
    const auto uri_to_file_path_iter = uri_to_file_path_mapping.find(request_path);
    const auto uri_is_mapped = uri_to_file_path_iter != uri_to_file_path_mapping.end();

    if (!uri_is_mapped)
    {
      spdlog::info("GET: URI is not mapped");
      request.reply(web::http::status_codes::NotFound);
      return;
    }

    const auto &file_path = uri_to_file_path_iter->second;
    auto open_file_stream_task = decltype(concurrency::streams::fstream::open_istream((const char *){})){};

    try
    {
      open_file_stream_task = concurrency::streams::fstream::open_istream(file_path);
    }
    catch (...)
    {
      spdlog::info("GET: cannot read requested file {}", file_path);
      request.reply(web::http::status_codes::NotFound);
      return;
    }

    open_file_stream_task.then(
        [request, content_type = ContentTypeFromFilePath(file_path)](const concurrency::streams::istream &input_stream)
        {
          request.reply(web::http::status_codes::OK, input_stream, content_type);
        });
  }
}

namespace stonks
{
  pplx::task<void> GetFileService::Start()
  {
    http_listener_ = web::http::experimental::listener::http_listener{BaseUri()};
    http_listener_.support(web::http::methods::GET,
                           [this](web::http::http_request request)
                           {
                             HandleGetRequest(*this, request);
                           });
    return http_listener_.open();
  }

  pplx::task<void> GetFileService::Stop()
  {
    return http_listener_.close();
  }

  const std::map<std::string, std::string> &GetFileService::UriToFilePathMapping() const
  {
    return uri_to_file_path_map_;
  }

  void GetFileService::SetUriToFilePathMapping(std::map<std::string, std::string> uri_to_file_path_map)
  {
    uri_to_file_path_map_ = std::move(uri_to_file_path_map);
  }
}