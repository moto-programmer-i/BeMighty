#include <iostream>
#include <vulkan/vulkan_raii.hpp>

int main() {
  int a = 2;
  std::cout << "ok" << std::endl;
  vk::raii::Image image = nullptr;
}
