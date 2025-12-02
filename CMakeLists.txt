cmake_minimum_required(VERSION 3.28)
project(MyVulkanEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Vulkan
find_package(Vulkan REQUIRED)
# Other dependencies
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(volk CONFIG REQUIRED)

add_executable(MyVulkanEngine
    src/main.cpp
)

target_include_directories(MyVulkanEngine PRIVATE include)
target_link_libraries(MyVulkanEngine PRIVATE Vulkan::Vulkan glfw glm::glm volk::volk)
