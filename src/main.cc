#include <iostream>
#include <optional>

#include "test_service.h"

int main()
{
  std::cout << stonks::f().value();
  return 0;
}