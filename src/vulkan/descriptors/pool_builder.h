#ifndef VKT_VULKAN_DESCRIPTORS_POOL_BUILDER_H
#define VKT_VULKAN_DESCRIPTORS_POOL_BUILDER_H

#include "../../util/list_helper.h"

#include "../context/context.h"

typedef struct {
    VKT_LIST_HELPER_STRUCT_MEMBERS(VkDescriptorPoolSize, sizes);
    uint32_t max_sets_count;
} VktDescriptorPoolBuilder;

VktDescriptorPoolBuilder vkt_descriptor_pool_builder_new();

void vkt_descriptor_pool_builder_push_size(VktDescriptorPoolBuilder *builder, VkDescriptorType type, uint32_t count);

int vkt_descriptor_pool_builder_build_pool(VktVulkanContext *context, VktDescriptorPoolBuilder *builder, VkDescriptorPool *pool);

void vkt_descriptor_pool_builder_destroy(VktDescriptorPoolBuilder *builder);

#endif /* VKT_VULKAN_DESCRIPTORS_POOL_BUILDER_H */
