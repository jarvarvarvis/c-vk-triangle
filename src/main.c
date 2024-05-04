#include "lib/lib_impls.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/context/context.h"
#include "vulkan/context/present_context.h"
#include "vulkan/swapchain/swapchain.h"
#include "vulkan/command/pool.h"
#include "vulkan/command/buffer.h"
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
    if (vkt_create_basic_graphics_context(&vk_context, "c-vk-triangle") != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create Vulkan context!");
        return EXIT_FAILURE;
    }

    // Create present context
    VktPresentContext present_context;
    if (vkt_create_present_context(&vk_context, &present_context, window) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create present context!");
        return EXIT_FAILURE;
    }

    // Create swapchain
    VkSwapchainKHR swapchain;
    if (vkt_create_swapchain(&vk_context, &present_context, &swapchain) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create swapchain!");
        return EXIT_FAILURE;
    }

    // Create command pool
    VkCommandPool test_cmd_pool;
    if (vkt_create_command_pool(&vk_context, &test_cmd_pool) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create graphics command pool!");
        return EXIT_FAILURE;
    }

    // Create test command buffer
    VkCommandBuffer test_buffer;
    if (vkt_allocate_primary_command_buffers(&vk_context, test_cmd_pool, &test_buffer, 1) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to allocate test command buffer!");
        return EXIT_FAILURE;
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    // Clean up
    vkt_free_command_buffers(&vk_context, test_cmd_pool, &test_buffer, 1);
    vkt_destroy_command_pool(&vk_context, test_cmd_pool);
    vkt_destroy_swapchain(&vk_context, swapchain);
    vkt_destroy_present_context(&vk_context, &present_context);
    vkt_destroy_context(&vk_context);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
