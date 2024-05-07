#ifndef VKT_VULKAN_PIPELINE_BUILDER_H
#define VKT_VULKAN_PIPELINE_BUILDER_H

#include <vulkan/vulkan.h>

#include "../context/context.h"

typedef struct {
    VkViewport viewport;
    VkRect2D scissor;

    VkPipelineLayout pipeline_layout;

    VkPipelineShaderStageCreateInfo *shader_stages;
    size_t shader_stages_len, shader_stages_cap;

    VkPipelineVertexInputStateCreateInfo vertex_input_state;
    VkPipelineInputAssemblyStateCreateInfo input_assembly_state;
    VkPipelineRasterizationStateCreateInfo rasterization_state;
    VkPipelineMultisampleStateCreateInfo multisampling_state;
    VkPipelineColorBlendAttachmentState color_blend_attachment_state;
} VktPipelineBuilder;

VktPipelineBuilder vkt_pipeline_builder_new();

void vkt_pipeline_builder_set_viewport(VktPipelineBuilder *builder, VkViewport viewport);
void vkt_pipeline_builder_set_viewport_from_extent(VktPipelineBuilder *builder, VkExtent2D extent);
void vkt_pipeline_builder_set_scissor(VktPipelineBuilder *builder, VkRect2D scissor);
void vkt_pipeline_builder_set_scissor_from_extent(VktPipelineBuilder *builder, VkExtent2D extent);

void vkt_pipeline_builder_set_pipeline_layout(VktPipelineBuilder *builder, VkPipelineLayout pipeline_layout);

void vkt_pipeline_builder_push_shader_stage(VktPipelineBuilder *builder, VkShaderStageFlagBits stage, VkShaderModule module);

void vkt_pipeline_builder_set_vertex_input_state(VktPipelineBuilder *builder);
void vkt_pipeline_builder_set_input_assembly_state(VktPipelineBuilder *builder, VkPrimitiveTopology topology);
void vkt_pipeline_builder_set_rasterization_state(VktPipelineBuilder *builder, VkPolygonMode polygon_mode);
void vkt_pipeline_builder_set_multisampling_state(VktPipelineBuilder *builder, VkSampleCountFlagBits samples);
void vkt_pipeline_builder_set_color_blend_attachment_state(VktPipelineBuilder *builder);

int vkt_pipeline_builder_build_pipeline(VktVulkanContext *context, VktPipelineBuilder *builder, VkRenderPass pass, VkPipeline *pipeline);

void vkt_pipeline_builder_destroy(VktPipelineBuilder *builder);

#endif /* VKT_VULKAN_PIPELINE_BUILDER_H */
