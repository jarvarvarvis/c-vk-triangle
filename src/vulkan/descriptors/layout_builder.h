#ifndef VKT_VULKAN_DESCRIPTORS_LAYOUT_BUILDER_H
#define VKT_VULKAN_DESCRIPTORS_LAYOUT_BUILDER_H

#include "../../util/list_helper.h"

#include "../context/context.h"

typedef struct {
    VKT_LIST_HELPER_STRUCT_MEMBERS(VkDescriptorSetLayoutBinding, bindings);
} VktDescriptorSetLayoutBuilder;

VktDescriptorSetLayoutBuilder vkt_descriptor_set_layout_builder_new();

void vkt_descriptor_set_layout_builder_push_layout_binding(VktDescriptorSetLayoutBuilder *builder, VkDescriptorSetLayoutBinding binding);
void vkt_descriptor_set_layout_builder_push_uniform_buffer_binding(VktDescriptorSetLayoutBuilder *builder, uint32_t binding_index, VkShaderStageFlags shader_stage);

int vkt_descriptor_set_layout_builder_build_layout(VktVulkanContext *context, VktDescriptorSetLayoutBuilder *builder, VkDescriptorSetLayout *layout);

void vkt_descriptor_set_layout_builder_destroy(VktDescriptorSetLayoutBuilder *builder);

#endif /* VKT_VULKAN_DESCRIPTORS_LAYOUT_BUILDER_H */
