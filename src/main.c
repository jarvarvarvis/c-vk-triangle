#include "lib/lib_impls.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/context/context.h"
#include "vulkan/common.h"

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

    // Create Vulkan context
    VktVulkanContext vk_context;
    if (vkt_create_basic_graphics_context(&vk_context, window) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create Vulkan context!");
        return EXIT_FAILURE;
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    // Clean up
    vkt_destroy_context(&vk_context);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
