#ifndef VKT_VULKAN_PIPELINE_LAYOUT_BUILDER_H
#define VKT_VULKAN_PIPELINE_LAYOUT_BUILDER_H

#include "../../util/list_helper.h"

#include "../context/context.h"

typedef struct {
    VKT_LIST_HELPER_STRUCT_MEMBERS(VkPushConstantRange, push_constants);
    VKT_LIST_HELPER_STRUCT_MEMBERS(VkDescriptorSetLayout, descriptor_set_layouts);
} VktPipelineLayoutBuilder;

VktPipelineLayoutBuilder vkt_pipeline_layout_builder_new();

void vkt_pipeline_layout_builder_push_push_constant_range(VktPipelineLayoutBuilder *builder, VkPushConstantRange range);
void vkt_pipeline_layout_builder_push_push_constant(VktPipelineLayoutBuilder *builder, uint32_t offset, uint32_t size, VkShaderStageFlags shader_stage);

void vkt_pipeline_layout_builder_push_descriptor_set_layout(VktPipelineLayoutBuilder *builder, VkDescriptorSetLayout layout);

int vkt_pipeline_layout_builder_build_layout(VktVulkanContext *context, VktPipelineLayoutBuilder *builder, VkPipelineLayout *layout);

void vkt_pipeline_layout_builder_destroy(VktPipelineLayoutBuilder *builder);

#endif /* VKT_VULKAN_PIPELINE_LAYOUT_BUILDER_H */
