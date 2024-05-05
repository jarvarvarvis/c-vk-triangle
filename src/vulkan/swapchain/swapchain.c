#include "swapchain.h"

#include "../common.h"
#include "../surface/surface.h"

#include <string.h>

int vkt_write_swapchain_create_info_image_format(VktVulkanContext *context, VkSurfaceKHR surface, VkSwapchainCreateInfoKHR *swapchain_info) {
    // Get surface formats
    VkSurfaceFormatKHR *surface_formats = NULL;
    uint32_t surface_format_count = 0;
    VKT_CHECK(vkt_query_supported_surface_formats(context, surface, &surface_formats, &surface_format_count));

    if (surface_format_count == 0) {
        c_log(C_LOG_SEVERITY_ERROR, "Unable to get supported formats for surface!");
        return VKT_GENERIC_FAILURE;
    }

    // Use the first available surface format
    swapchain_info->imageFormat = surface_formats[0].format;
    swapchain_info->imageColorSpace = surface_formats[0].colorSpace;

    // Clean up
    free(surface_formats);

    return VKT_GENERIC_SUCCESS;
}

int vkt_write_swapchain_create_info_surface_capabilities(VktVulkanContext *context, VkSurfaceKHR surface, VkSwapchainCreateInfoKHR *swapchain_info) {
    // Get surface capabilities
    VkSurfaceCapabilitiesKHR surface_capabilities;
    VKT_CHECK(vkt_query_surface_capabilities(context, surface, &surface_capabilities));

    // Write swapchain info
    swapchain_info->minImageCount = surface_capabilities.minImageCount;
    swapchain_info->imageExtent = surface_capabilities.currentExtent;

    // Get pre transform
    VkSurfaceTransformFlagBitsKHR desired_transforms = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    if (surface_capabilities.supportedTransforms & desired_transforms) {
        swapchain_info->preTransform = desired_transforms;
    } else {
        swapchain_info->preTransform = surface_capabilities.currentTransform;
    }

    return VKT_GENERIC_SUCCESS;
}

int vkt_write_swapchain_create_info_present_mode(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VkSwapchainCreateInfoKHR *swapchain_info,
    VkPresentModeKHR desired_present_mode
) {
    // Get present modes
    VkPresentModeKHR *present_modes = NULL;
    uint32_t present_mode_count = 0;
    VKT_CHECK(vkt_query_surface_present_modes(context, surface, &present_modes, &present_mode_count));

    if (present_mode_count == 0) {
        c_log(C_LOG_SEVERITY_ERROR, "Unable to get present modes for surface!");
        return VKT_GENERIC_FAILURE;
    }

    // Get index for the desired present mode (or set index for fallback VK_PRESENT_MODE_FIFO_KHR)
    const VkPresentModeKHR FALLBACK_PRESENT_MODE = VK_PRESENT_MODE_FIFO_KHR;
    int present_mode_idx = -1;
    int fallback_present_mode_idx = -1;
    for (int i = 0; i < present_mode_count; ++i) {
        VkPresentModeKHR present_mode = present_modes[i];
        if (present_mode == desired_present_mode) {
            present_mode_idx = i;
        }
        if (present_mode == FALLBACK_PRESENT_MODE) {
            fallback_present_mode_idx = i;
        }
    }

    free(present_modes);

    if (fallback_present_mode_idx == -1) {
        c_log(C_LOG_SEVERITY_ERROR, "Fallback VK_PRESENT_MODE_FIFO_KHR is not supported, something is wrong!");
        return VKT_GENERIC_FAILURE;
    }

    // Set the desired present mode if it is supported and FALLBACK_PRESENT_MODE otherwise
    swapchain_info->presentMode = (present_mode_idx != -1 ? desired_present_mode : FALLBACK_PRESENT_MODE);

    return VKT_GENERIC_SUCCESS;
}

int vkt_create_swapchain(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VktSwapchainCreateProps props,
    VkSwapchainKHR *swapchain
) {
    VkSwapchainCreateInfoKHR swapchain_info;
    memset(&swapchain_info, 0, sizeof(VkSwapchainCreateInfoKHR));

    swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_info.pNext = NULL;

    swapchain_info.surface = surface;

    VKT_CHECK(vkt_write_swapchain_create_info_image_format(context, surface, &swapchain_info));

    // Some specific swapchain image flags
    swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    swapchain_info.imageArrayLayers = 1;

    VKT_CHECK(vkt_write_swapchain_create_info_surface_capabilities(context, surface, &swapchain_info));

    VKT_CHECK(vkt_write_swapchain_create_info_present_mode(context, surface, &swapchain_info, props.desired_present_mode));

    // Finally create the swapchain
    VKT_CHECK(vkCreateSwapchainKHR(context->logical_device.vk_device, &swapchain_info, NULL, swapchain));

    c_log(C_LOG_SEVERITY_DEBUG, "Created the swapchain with:");
    c_log(C_LOG_SEVERITY_DEBUG, "- Present mode: %s", string_VkPresentModeKHR(swapchain_info.presentMode));
    c_log(C_LOG_SEVERITY_DEBUG, "- Pre transform: %s", string_VkSurfaceTransformFlagBitsKHR(swapchain_info.preTransform));
    c_log(C_LOG_SEVERITY_DEBUG, "- Image format: %s", string_VkFormat(swapchain_info.imageFormat));
    c_log(C_LOG_SEVERITY_DEBUG, "- Image extent: %d x %d", swapchain_info.imageExtent.width, swapchain_info.imageExtent.height);

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_swapchain(VktVulkanContext *context, VkSwapchainKHR swapchain) {
    vkDestroySwapchainKHR(context->logical_device.vk_device, swapchain, NULL);
}
