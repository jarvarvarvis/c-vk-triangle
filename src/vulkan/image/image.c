#include "image.h"

#include "../common.h"

VkImageCreateInfo vkt_get_2d_image_create_info(VkFormat format, VkImageUsageFlags usage, VkExtent3D extent) {
    VkImageCreateInfo info;
    memset(&info, 0, sizeof(VkImageCreateInfo));

    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.pNext = NULL;

    info.imageType = VK_IMAGE_TYPE_2D;

    info.format = format;
    info.extent = extent;

    info.mipLevels = 1;
    info.arrayLayers = 1;
    info.samples = VK_SAMPLE_COUNT_1_BIT;
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.usage = usage;

    return info;
}

VkImageViewCreateInfo vkt_get_2d_image_view_create_info(VkFormat format, VkImage image, VkImageAspectFlags aspect_flags) {
    VkImageViewCreateInfo info;
    memset(&info, 0, sizeof(VkImageViewCreateInfo));

    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.pNext = NULL;

    info.viewType = VK_IMAGE_VIEW_TYPE_2D;

    info.image = image;
    info.format = format;
    info.subresourceRange.baseMipLevel = 0;
    info.subresourceRange.levelCount = 1;
    info.subresourceRange.baseArrayLayer = 0;
    info.subresourceRange.layerCount = 1;
    info.subresourceRange.aspectMask = aspect_flags;

    return info;
}

int vkt_create_image_view(VktVulkanContext *context, VkImageViewCreateInfo info, VkImageView *view) {
    VKT_CHECK(vkCreateImageView(
        context->logical_device.vk_device, &info, NULL, view
    ));
    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_image_view(VktVulkanContext *context, VkImageView view) {
    vkDestroyImageView(context->logical_device.vk_device, view, NULL);
}

int vkt_allocate_gpu_only_image(VktVulkanContext *context, VkImageCreateInfo info, VktAllocatedImage *image) {
    memset(image, 0, sizeof(VktAllocatedImage));

    VmaAllocationCreateInfo alloc_info;
    memset(&alloc_info, 0, sizeof(VmaAllocationCreateInfo));

    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
    alloc_info.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    alloc_info.priority = 1.0f;

    // Create the image
    VKT_CHECK(vmaCreateImage(
        context->allocator,
        &info,
        &alloc_info,
        &image->image,
        &image->allocation,
        NULL
    ));
    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_image(VktVulkanContext *context, VktAllocatedImage *image) {
    vmaDestroyImage(context->allocator, image->image, image->allocation);
}
