#ifndef STONKS_GET_FILE_SERVICE
#define STONKS_GET_FILE_SERVICE

#include <string>
#include <map>

#include <cpprest/http_listener.h>
#include <pplx/pplxtasks.h>

namespace stonks
{
  class GetFileService
  {
  public:
    pplx::task<void> Start();
    pplx::task<void> Stop();

    const std::map<std::string, std::string> &UriToFilePathMapping() const;
    void SetUriToFilePathMapping(std::map<std::string, std::string> uri_to_file_path_map);

  private:
    web::http::experimental::listener::http_listener http_listener_;
    std::map<std::string, std::string> uri_to_file_path_map_;
  };
}

#endif // STONKS_GET_FILE_SERVICE