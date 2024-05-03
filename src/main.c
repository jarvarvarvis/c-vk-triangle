#include "lib/lib_impls.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/common.h"
#include "vulkan/instance/instance.h"
#include "vulkan/device/physical.h"
#include "vulkan/device/logical.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

int main() {
    // Initialize glfw
    if (!glfwInit()) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to initialize glfw!");
        return EXIT_FAILURE;
    }

    // Create the window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "c-vk-triangle", NULL, NULL);
    if (!window) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create glfw window!");
        return EXIT_FAILURE;
    }

    // Create instance
    VktVulkanInstance instance;
    if (vkt_create_vulkan_instance(&instance, window) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create Vulkan instance!");
        return EXIT_FAILURE;
    }

    // Physical device discovery
    VktFindPhysicalDeviceResult phys_device_result;
    VktFindPhysicalDeviceProps find_phys_device_props;
    find_phys_device_props.queue_family_bits = VK_QUEUE_GRAPHICS_BIT;
    if (vkt_find_physical_device(&instance, find_phys_device_props, &phys_device_result) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Unable to find physical device!");
        return EXIT_FAILURE;
    }

    // Create logical device
    VktLogicalDevice device;
    if (vkt_create_logical_device(&device, phys_device_result) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Unable to create logical device!");
        return EXIT_FAILURE;
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    // Clean up
    vkt_destroy_logical_device(&device);
    vkt_destroy_vulkan_instance(&instance);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
