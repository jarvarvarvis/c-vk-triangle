#ifndef VKT_VULKAN_IMAGE_H
#define VKT_VULKAN_IMAGE_H

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "../context/context.h"

VkImageCreateInfo vkt_get_2d_image_create_info(VkFormat format, VkImageUsageFlags usage, VkExtent3D extent);
VkImageViewCreateInfo vkt_get_2d_image_view_create_info(VkFormat format, VkImage image, VkImageAspectFlags aspect_flags);

int vkt_create_image_view(VktVulkanContext *context, VkImageViewCreateInfo info, VkImageView *view);
void vkt_destroy_image_view(VktVulkanContext *context, VkImageView view);

typedef struct {
    VkImage image;
    VmaAllocation allocation;
} VktAllocatedImage;

int vkt_allocate_gpu_only_image(VktVulkanContext *context, VkImageCreateInfo info, VktAllocatedImage *image);
void vkt_destroy_image(VktVulkanContext *context, VktAllocatedImage *image);

#endif /* VKT_VULKAN_IMAGE_H */
