#include <iostream>
#include <exception>

#include "get_service.h"

int main(int, const char *[])
{
  auto service_controller = stonks::GetService{};

  try
  {
    service_controller.run().wait();
    std::cout << "stonks: Started listening on: " << service_controller.endpoint() << std::endl;
    getchar();
    service_controller.stop().wait();
  }
  catch (const std::exception &e)
  {
    std::cerr << "stonks: Error: " << e.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "stonks: Unknown error" << std::endl;
  }

  return 0;
}