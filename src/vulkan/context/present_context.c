#include "present_context.h"

#include "../common.h"
#include "../renderpass/renderpass.h"

#include <c_log/c_log.h>

int vkt_create_present_context(VktVulkanContext *context, VktPresentContext *present_context, GLFWwindow *window) {
    memset(present_context, 0, sizeof(VktPresentContext));

    // Create window surface
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(context->instance.vk_instance, window, NULL, &surface) != VK_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create window surface!");
        return VKT_GENERIC_FAILURE;
    }

    // Validate that the physical device and the selected queue support the surface
    VkBool32 surface_supported_by_device_and_queue = VK_TRUE;
    vkGetPhysicalDeviceSurfaceSupportKHR(
        context->physical_device.vk_device,
        context->physical_device.queue_family_index,
        surface,
        &surface_supported_by_device_and_queue
    );
    if (!surface_supported_by_device_and_queue) {
        c_log(C_LOG_SEVERITY_ERROR, "Window surface created by GLFW not supported by selected physical device and queue!");
        vkDestroySurfaceKHR(context->instance.vk_instance, surface, NULL);
        return VKT_GENERIC_FAILURE;
    }

    present_context->surface = surface;

    // Query surface information
    if (vkt_query_surface_info(context, surface, &present_context->surface_info) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to query information for window surface!");
        vkDestroySurfaceKHR(context->instance.vk_instance, surface, NULL);
        return VKT_GENERIC_FAILURE;
    }

    return VKT_GENERIC_SUCCESS;
}

int vkt_create_present_context_swapchain(VktVulkanContext *context, VktPresentContext *present_context, VktSwapchainCreateProps props) {
    memset(&present_context->swapchain, 0, sizeof(VkSwapchainKHR));
    memset(&present_context->swapchain_images, 0, sizeof(VktSwapchainImages));

    // Create the swapchain
    VKT_CHECK(vkt_create_swapchain(
        context,
        present_context->surface,
        &present_context->surface_info,
        props,
        &present_context->swapchain
    ));

    // Create swapchain images
    VKT_CHECK(vkt_create_swapchain_images(
        context,
        present_context->swapchain,
        present_context->surface_info.surface_formats[0].format,
        &present_context->swapchain_images
    ));

    return VKT_GENERIC_SUCCESS;
}

int vkt_create_present_context_render_pass(VktVulkanContext *context, VktPresentContext *present_context) {
    memset(&present_context->render_pass, 0, sizeof(VkRenderPass));

    // Create the render pass
    VKT_CHECK(vkt_create_default_renderpass(
        context,
        present_context->surface_info.surface_formats[0].format,
        &present_context->render_pass
    ));

    return VKT_GENERIC_SUCCESS;
}

int vkt_create_present_context_framebuffers(VktVulkanContext *context, VktPresentContext *present_context) {
    memset(&present_context->framebuffers, 0, sizeof(VktFramebuffers));

    // Create the framebuffers
    VKT_CHECK(vkt_create_framebuffers(
        context,
        &present_context->swapchain_images,
        present_context->render_pass,
        present_context->surface_info.surface_capabilities.currentExtent,
        &present_context->framebuffers
    ));

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_present_context(VktVulkanContext *context, VktPresentContext *present_context) {
    vkt_destroy_framebuffers(context, &present_context->framebuffers);
    vkt_destroy_renderpass(context, present_context->render_pass);

    vkt_destroy_swapchain_images(context, &present_context->swapchain_images);
    vkt_destroy_swapchain(context, present_context->swapchain);

    vkDestroySurfaceKHR(context->instance.vk_instance, present_context->surface, NULL);
    vkt_destroy_surface_info(&present_context->surface_info);
}
