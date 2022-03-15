#include <iostream>

#include <spdlog/spdlog.h>

#include "get_file_service.h"

int main(int, const char *[])
{
  auto get_file_service_controller = stonks::GetFileService{};
  get_file_service_controller.SetUriToFilePathMapping(
      {
        {"/", "../web/index.html"},
        {"/styles1.css", "../web/styles.css"}
      });

  get_file_service_controller.Start().wait();
  spdlog::info("Started");
  getchar();
  get_file_service_controller.Stop().wait();

  // auto get_file_service_controller1 = stonks::GetFileService{};
  // get_file_service_controller1.SetUriToFilePathMapping(
  //     {{"/", "../web/index.html"}});

  // auto get_file_service_controller2 = stonks::GetFileService{};
  // get_file_service_controller2.SetUriToFilePathMapping(
  //     {{"/styles1.css", "../web/styles.css"}});

  // get_file_service_controller1.Start().wait();
  // get_file_service_controller2.Start().wait();
  // spdlog::info("Started");
  // getchar();
  // get_file_service_controller1.Stop().wait();
  // get_file_service_controller2.Stop().wait();

  return 0;
}