#include "buffer.h"

#include "../common.h"

int vkt_allocate_buffer(VktEngine *engine, VkBufferUsageFlags usage, size_t size, VktAllocatedBuffer *buffer) {
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

    alloc_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    // Create the buffer
    VKT_CHECK(vmaCreateBuffer(
        engine->allocator,
        &buffer_info,
        &alloc_info,
        &buffer->buffer,
        &buffer->allocation,
        NULL
    ));
    return VKT_GENERIC_SUCCESS;
}

int vkt_upload_to_buffer(VktEngine *engine, VktAllocatedBuffer *buffer, void *data, size_t size) {
    void *mapped_ptr;
    VKT_CHECK(vmaMapMemory(engine->allocator, buffer->allocation, &mapped_ptr));

    memcpy(mapped_ptr, data, size);

    vmaUnmapMemory(engine->allocator, buffer->allocation);

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_buffer(VktEngine *engine, VktAllocatedBuffer *buffer) {
    vmaDestroyBuffer(engine->allocator, buffer->buffer, buffer->allocation);
}
