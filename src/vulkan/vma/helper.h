#ifndef VKT_LIB_VMA_HELPER_H
#define VKT_LIB_VMA_HELPER_H

#include "../context/context.h"

#include <c_log/c_log.h>

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include <vk_mem_alloc.h>

int vkt_vma_helper_create_allocator(VktVulkanContext *context, VmaAllocator *allocator);

#endif /* VKT_LIB_VMA_HELPER_H */
