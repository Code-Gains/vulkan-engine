#include <volk.h>
#include <GLFW/glfw3.h>
#include <vkbootstrap/VkBootstrap.h>
#include <iostream>
#include <vector>

VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("No Vulkan GPUs found.");
    }
    else
        std::cout << "Found " << deviceCount  << " devices\n";

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    VkPhysicalDevice selectedDevice = VK_NULL_HANDLE;

    for (const auto& device : devices) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        std::cout << "  Device Name: " << deviceProperties.deviceName << "\n";
        std::cout << "  Device Type: ";
        switch (deviceProperties.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: std::cout << "Integrated GPU"; break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: std::cout << "Discrete GPU"; break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: std::cout << "Virtual GPU"; break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU: std::cout << "CPU"; break;
        default: std::cout << "Other"; break;
        }
        std::cout << "\n";

        // Pick the first discrete GPU if available, otherwise first device
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && selectedDevice == VK_NULL_HANDLE) {
            selectedDevice = device;
        }
        if (selectedDevice == VK_NULL_HANDLE)
            selectedDevice = device;
    }

    return selectedDevice;
}

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

    // GLFW parameters
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    // Vulkan
    VkInstance instance;
    VkApplicationInfo appInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
    appInfo.pApplicationName = "Example App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    createInfo.pApplicationInfo = &appInfo;

    // Get GLFW required extensions
    uint32_t glfwExtCount = 0;
    const char** glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtCount);
    createInfo.enabledExtensionCount = glfwExtCount;
    createInfo.ppEnabledExtensionNames = glfwExts;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance\n";
        return -1;
    }

    volkLoadInstance(instance);

    // Create a Vulkan surface for the GLFW window
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        std::cerr << "Failed to create window surface\n";
        return -1;
    }

    // 1. Physical device selection -> Select device usually at index 0 (Nvidia/AMD)
    VkPhysicalDevice pickPhysicalDevice(instance, surface);
    // 2. Logical device creation -> need some sort of logical device API (that calls physical device through Vulkan -> drivers)
    
    // 3. Swapchain creation -> Not sure how Vulkan swapchain works (is it dual, multiple? Find out how textures work in general)
    // 4. Image views and render pass ??
    // 5. Framebuffers
    // 6. Command buffers to clear the image -> Reset Frame

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // Poll Inputs
    }

    // Destroy On Terminate
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// VK bootstrap header
// Vulkan Guide VKGuide.dev

