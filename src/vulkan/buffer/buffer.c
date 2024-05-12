#include "buffer.h"

#include "../common.h"

int vkt_allocate_buffer_for_uploads(VktVulkanContext *context, VkBufferUsageFlags usage, size_t size, VktAllocatedBuffer *buffer) {
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

    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;

    // This is a buffer that we sequentially copy data into
    alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

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
