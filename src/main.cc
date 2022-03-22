#include <variant>
#include <vector>

#include <pplx/pplxtasks.h>
#include <spdlog/spdlog.h>

#include "get_file_service.h"
#include "get_symbols_service.h"

int main(int, char const *[])
{
  using ServiceVariant = std::variant<
      stonks::GetFileService,
      stonks::GetSymbolsService>;

  auto services = std::vector<ServiceVariant>{};
  services.emplace_back(stonks::GetFileService{});
  services.emplace_back(stonks::GetSymbolsService{});

  for (auto &service : services)
  {
    std::visit(
        [](auto &service)
        {
          service.Start();
        },
        service);
  }

  spdlog::info("Started {} services", services.size());
  getchar();
  spdlog::info("Stopping...");

  auto stop_tasks = std::vector<pplx::task<void>>{};

  for (auto &service : services)
  {
    auto stop_task = std::visit(
        [](auto &service)
        {
          return service.Stop();
        },
        service);
    stop_tasks.emplace_back(std::move(stop_task));
  }

  for (auto &stop_task : stop_tasks)
  {
    stop_task.wait();
  }

  spdlog::info("Stopped");

  return 0;
}