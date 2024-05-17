#include "buffer.h"

#include "../common.h"

int vkt_allocate_buffer(
    VktVulkanContext *context,
    VkBufferUsageFlags usage,
    size_t size,
    VmaMemoryUsage memory_usage,
    VmaAllocationCreateFlags allocation_flags,
    VktAllocatedBuffer *buffer
) {
    memset(buffer, 0, sizeof(VktAllocatedBuffer));

    // Create buffer info
    VkBufferCreateInfo buffer_info;
    memset(&buffer_info, 0, sizeof(VkBufferCreateInfo));

    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.pNext = NULL;

    buffer_info.size = size;
    buffer_info.usage = usage;

    // Create allocation info
    VmaAllocationCreateInfo alloc_info;
    memset(&alloc_info, 0, sizeof(VmaAllocationCreateInfo));

    alloc_info.usage = memory_usage;

    alloc_info.flags = allocation_flags;

    // Create the buffer
    VKT_CHECK(vmaCreateBuffer(
        context->allocator,
        &buffer_info,
        &alloc_info,
        &buffer->buffer,
        &buffer->allocation,
        NULL
    ));
    return VKT_GENERIC_SUCCESS;
}

int vkt_allocate_buffer_for_uploads(VktVulkanContext *context, VkBufferUsageFlags usage, size_t size, VktAllocatedBuffer *buffer) {
    // VMA_MEMORY_USAGE_AUTO: Automatically select memory type for the allocation
    // VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT: Specify that the buffer's memory can be mapped and that data 
    // is sequentially copied into it (in our case using memcpy)
    return vkt_allocate_buffer(context, usage, size, VMA_MEMORY_USAGE_AUTO, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT, buffer);
}

int vkt_upload_to_buffer(VktVulkanContext *context, VktAllocatedBuffer *buffer, void *data, size_t size) {
    void *mapped_ptr;
    VKT_CHECK(vmaMapMemory(context->allocator, buffer->allocation, &mapped_ptr));

    memcpy(mapped_ptr, data, size);

    vmaUnmapMemory(context->allocator, buffer->allocation);

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_buffer(VktVulkanContext *context, VktAllocatedBuffer *buffer) {
    vmaDestroyBuffer(context->allocator, buffer->buffer, buffer->allocation);
}
