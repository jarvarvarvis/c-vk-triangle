#ifndef VKT_VULKAN_DESCRIPTORS_UPDATE_BUILDER_H
#define VKT_VULKAN_DESCRIPTORS_UPDATE_BUILDER_H

#include <c_utils_collection/datatypes/list_helper.h> 

#include "../context/context.h"

typedef struct {
    LIST_HELPER_STRUCT_MEMBERS(VkDescriptorBufferInfo, buffer_infos);
    LIST_HELPER_STRUCT_MEMBERS(VkWriteDescriptorSet, set_writes);

    LIST_HELPER_STRUCT_MEMBERS(VkCopyDescriptorSet, set_copies);
} VktDescriptorSetUpdateBuilder;

VktDescriptorSetUpdateBuilder vkt_descriptor_set_update_builder_new();

void vkt_descriptor_set_update_builder_push_write(VktDescriptorSetUpdateBuilder *builder, VkWriteDescriptorSet write);
void vkt_descriptor_set_update_builder_push_buffer_write(VktDescriptorSetUpdateBuilder *builder, VkBuffer buffer, VkDeviceSize buffer_size, VkDescriptorSet set, uint32_t binding, VkDescriptorType descriptor_type);
void vkt_descriptor_set_update_builder_push_copy(VktDescriptorSetUpdateBuilder *builder, VkCopyDescriptorSet copy);

void vkt_descriptor_set_update_builder_update_sets(VktVulkanContext *context, VktDescriptorSetUpdateBuilder *builder);

void vkt_descriptor_set_update_builder_destroy(VktDescriptorSetUpdateBuilder *builder);

#endif /* VKT_VULKAN_DESCRIPTORS_UPDATE_BUILDER_H */
