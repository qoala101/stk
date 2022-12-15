#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

auto main(int argc, const char *const *argv) -> int {
  std::cout << "start\n";

  if (std::filesystem::is_directory("/app/bin/new_dir1")) {
    std::cout << "new_dir exists\n";
  } else {
    if (std::filesystem::create_directory("/app/bin/new_dir1")) {
      std::cout << "created dir new_dir\n";
    } else {
      std::cout << "failed to create dir new_dir\n";
    }
  }

  if (std::filesystem::is_regular_file("/app/bin/new_dir1/new_file.txt")) {
    std::cout << "new_file.txt exists\n";
  } else {
    auto stream = std::ofstream{"/app/bin/new_dir1/new_file.txt"};
    stream << "Hello, World\n" << std::endl;
    std::cout << "created new_file.txt\n";
  }

  std::cout << "end sleeping...\n";
  std::this_thread::sleep_for(std::chrono::hours{1});
}