#include "images.h"

#include "../common.h"

int vkt_create_swapchain_image_view(
    VktVulkanContext *context,
    VkFormat format,
    VkImage image,
    VkImageView *view
) {
    VkImageViewCreateInfo create_info;
    memset(&create_info, 0, sizeof(VkImageViewCreateInfo));

    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.pNext = NULL;

    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = format;

    VkComponentMapping component_mapping;
    component_mapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    component_mapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    component_mapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    component_mapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components = component_mapping;

    VkImageSubresourceRange subresource_range;
    subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresource_range.baseMipLevel = 0;
    subresource_range.levelCount = 1;
    subresource_range.baseArrayLayer = 0;
    subresource_range.layerCount = 1;
    create_info.subresourceRange = subresource_range;

    create_info.image = image;

    // Create the image view
    VKT_CHECK(vkCreateImageView(context->logical_device.vk_device, &create_info, NULL, view));
    return VKT_GENERIC_SUCCESS;
}

int vkt_create_swapchain_images(
    VktVulkanContext *context,
    VkSwapchainKHR swapchain,
    VkFormat format,
    VktSwapchainImages *images
) {
    // Get swapchain images
    VKT_CHECK(vkGetSwapchainImagesKHR(context->logical_device.vk_device, swapchain, &images->image_count, NULL));

    images->images = malloc(sizeof(VkImage) * images->image_count);
    VKT_CHECK(vkGetSwapchainImagesKHR(context->logical_device.vk_device, swapchain, &images->image_count, images->images));

    // Create swapchain image views
    images->image_views = malloc(sizeof(VkImageView) * images->image_count);
    for (uint32_t i = 0; i < images->image_count; ++i) {
        VKT_CHECK(vkt_create_swapchain_image_view(context, format, images->images[i], &images->image_views[i]));
    }

    c_log(C_LOG_SEVERITY_DEBUG, "Got %d swapchain images", images->image_count);
    c_log(C_LOG_SEVERITY_DEBUG, "- Created image views with format: %s", string_VkFormat(format));
    printf("\n");

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_swapchain_images(VktVulkanContext *context, VktSwapchainImages *images) {
    for (uint32_t i = 0; i < images->image_count; ++i) {
        vkDestroyImageView(context->logical_device.vk_device, images->image_views[i], NULL);
    }
    free(images->image_views);
    free(images->images);
}
