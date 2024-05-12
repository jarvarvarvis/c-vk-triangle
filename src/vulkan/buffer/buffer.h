#ifndef VKT_VULKAN_BUFFER_H
#define VKT_VULKAN_BUFFER_H

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "../context/context.h"

typedef struct {
    VkBuffer buffer;
    VmaAllocation allocation;
} VktAllocatedBuffer;

int vkt_allocate_buffer_for_uploads(VktVulkanContext *context, VkBufferUsageFlags usage, size_t size, VktAllocatedBuffer *buffer);
int vkt_upload_to_buffer(VktVulkanContext *context, VktAllocatedBuffer *buffer, void *data, size_t size);
void vkt_destroy_buffer(VktVulkanContext *context, VktAllocatedBuffer *buffer);

#endif /* VKT_VULKAN_BUFFER_H */
