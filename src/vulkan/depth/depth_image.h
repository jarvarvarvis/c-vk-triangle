#ifndef VKT_VULKAN_DEPTH_IMAGE_H
#define VKT_VULKAN_DEPTH_IMAGE_H

#include <vulkan/vulkan.h>

#include "../image/image.h"

typedef struct {
    VktAllocatedImage image;
    VkImageView image_view;

    VkFormat format;
} VktDepthImage;

int vkt_create_depth_image(VktVulkanContext *context, VkExtent2D initial_extent, VktDepthImage *depth_image);
void vkt_destroy_depth_image(VktVulkanContext *context, VktDepthImage *depth_image);

#endif /* VKT_VULKAN_DEPTH_IMAGE_H */
