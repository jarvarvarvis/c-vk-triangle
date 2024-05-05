#include "swapchain.h"

#include "../common.h"

#include <string.h>

int vkt_write_swapchain_create_info_image_format(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VkSurfaceFormatKHR surface_format,
    VkSwapchainCreateInfoKHR *swapchain_info
) {
    swapchain_info->imageFormat = surface_format.format;
    swapchain_info->imageColorSpace = surface_format.colorSpace;

    return VKT_GENERIC_SUCCESS;
}

int vkt_write_swapchain_create_info_surface_capabilities(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VkSurfaceCapabilitiesKHR surface_capabilities,
    VkSwapchainCreateInfoKHR *swapchain_info
) {
    // Write swapchain info
    swapchain_info->minImageCount = surface_capabilities.minImageCount;
    swapchain_info->imageExtent = surface_capabilities.currentExtent;

    // Write pre transform
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
    VktSurfaceInfo *surface_info,
    VkSwapchainCreateInfoKHR *swapchain_info,
    VkPresentModeKHR desired_present_mode
) {
    // Get index for the desired present mode (or set index for fallback VK_PRESENT_MODE_FIFO_KHR)
    const VkPresentModeKHR FALLBACK_PRESENT_MODE = VK_PRESENT_MODE_FIFO_KHR;
    int present_mode_idx = -1;
    int fallback_present_mode_idx = -1;
    for (int i = 0; i < surface_info->present_mode_count; ++i) {
        VkPresentModeKHR present_mode = surface_info->present_modes[i];
        if (present_mode == desired_present_mode) {
            present_mode_idx = i;
        }
        if (present_mode == FALLBACK_PRESENT_MODE) {
            fallback_present_mode_idx = i;
        }
    }

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
    VktSurfaceInfo *surface_info,
    VktSwapchainCreateProps props,
    VkSwapchainKHR *swapchain
) {
    VkSwapchainCreateInfoKHR swapchain_info;
    memset(&swapchain_info, 0, sizeof(VkSwapchainCreateInfoKHR));

    swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_info.pNext = NULL;

    swapchain_info.surface = surface;

    VKT_CHECK(vkt_write_swapchain_create_info_image_format(context, surface, surface_info->surface_formats[0], &swapchain_info));

    // Some specific swapchain image flags
    swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    swapchain_info.imageArrayLayers = 1;

    VKT_CHECK(vkt_write_swapchain_create_info_surface_capabilities(context, surface, surface_info->surface_capabilities, &swapchain_info));

    VKT_CHECK(vkt_write_swapchain_create_info_present_mode(context, surface, surface_info, &swapchain_info, props.desired_present_mode));

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
