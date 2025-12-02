#include <volk.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    if (!glfwVulkanSupported()) {
        std::cerr << "Vulkan not supported\n";
        return -1;
    }

    std::cout << "Vulkan is supported! Starting volk...\n";

    volkInitialize();

    glfwTerminate();
    return 0;
}
