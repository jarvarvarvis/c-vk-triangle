#include "present_context.h"

#include "../common.h"

#include <c_log/c_log.h>

int vkt_create_present_context(VktVulkanContext *context, VktPresentContext *present_context, GLFWwindow *window) {
    memset(present_context, 0, sizeof(VktPresentContext));

    // Create window surface
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(context->instance.vk_instance, window, NULL, &surface) != VK_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create window surface!");
        return VKT_GENERIC_FAILURE;
    }

    // Validate that the physical device supports the surface
    VkBool32 surface_supported_by_device_and_queue = VK_TRUE;
    vkGetPhysicalDeviceSurfaceSupportKHR(
        context->physical_device.vk_device,
        context->physical_device.queue_family_index,
        surface,
        &surface_supported_by_device_and_queue
    );
    if (!surface_supported_by_device_and_queue) {
        c_log(C_LOG_SEVERITY_ERROR, "Window surface created by GLFW not supported by selected physical device and queue!");
        return VKT_GENERIC_FAILURE;
    }

    present_context->surface = surface;

    return VKT_GENERIC_SUCCESS;
}

int vkt_create_present_context_swapchain(VktVulkanContext *context, VktPresentContext *present_context, VktSwapchainCreateProps props) {
    memset(&present_context->swapchain, 0, sizeof(VkSwapchainKHR));
    VKT_CHECK(vkt_create_swapchain(context, present_context->surface, props, &present_context->swapchain));

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_present_context(VktVulkanContext *context, VktPresentContext *present_context) {
    vkt_destroy_swapchain(context, present_context->swapchain);
    vkDestroySurfaceKHR(context->instance.vk_instance, present_context->surface, NULL);
}
