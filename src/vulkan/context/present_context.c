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

    int width = 0, height = 0;
    glfwGetWindowSize(window, &width, &height);

    present_context->image_size.width = width;
    present_context->image_size.height = height;

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
    memset(&present_context->main_render_pass, 0, sizeof(VkRenderPass));

    // Create the render pass
    VKT_CHECK(vkt_create_default_renderpass(
        context,
        present_context->surface_info.surface_formats[0].format,
        &present_context->main_render_pass
    ));

    return VKT_GENERIC_SUCCESS;
}

int vkt_create_present_context_framebuffers(VktVulkanContext *context, VktPresentContext *present_context) {
    memset(&present_context->framebuffers, 0, sizeof(VktFramebuffers));

    // Create the framebuffers
    VKT_CHECK(vkt_create_framebuffers(
        context,
        &present_context->swapchain_images,
        present_context->main_render_pass,
        vkt_present_context_get_swapchain_extent(present_context),
        &present_context->framebuffers
    ));

    return VKT_GENERIC_SUCCESS;
}

VkExtent2D vkt_present_context_get_swapchain_extent(VktPresentContext *present_context) {
    VkExtent2D swapchain_extent;
    VkSurfaceCapabilitiesKHR surface_capabilities = present_context->surface_info.surface_capabilities;

    // width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
    if (surface_capabilities.currentExtent.width == 0xFFFFFFFF) {
        // If the surface size is undefined, the size is set to the size
        // of the images requested, which must fit within the minimum and
        // maximum values.
        swapchain_extent.width = present_context->image_size.width;
        swapchain_extent.height = present_context->image_size.height;

        if (swapchain_extent.width < surface_capabilities.minImageExtent.width) {
            swapchain_extent.width = surface_capabilities.minImageExtent.width;
        } else if (swapchain_extent.width > surface_capabilities.maxImageExtent.width) {
            swapchain_extent.width = surface_capabilities.maxImageExtent.width;
        }

        if (swapchain_extent.height < surface_capabilities.minImageExtent.height) {
            swapchain_extent.height = surface_capabilities.minImageExtent.height;
        } else if (swapchain_extent.height > surface_capabilities.maxImageExtent.height) {
            swapchain_extent.height = surface_capabilities.maxImageExtent.height;
        }
    } else {
        // If the surface size is defined, the swap chain size must match
        swapchain_extent = surface_capabilities.currentExtent;
        present_context->image_size.width = surface_capabilities.currentExtent.width;
        present_context->image_size.height = surface_capabilities.currentExtent.height;
    }

    return swapchain_extent;
}

void vkt_destroy_present_context(VktVulkanContext *context, VktPresentContext *present_context) {
    vkt_destroy_framebuffers(context, &present_context->framebuffers);
    vkt_destroy_renderpass(context, present_context->main_render_pass);

    vkt_destroy_swapchain_images(context, &present_context->swapchain_images);
    vkt_destroy_swapchain(context, present_context->swapchain);

    vkDestroySurfaceKHR(context->instance.vk_instance, present_context->surface, NULL);
    vkt_destroy_surface_info(&present_context->surface_info);
}
