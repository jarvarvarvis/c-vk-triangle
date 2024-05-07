#include "builder.h"

#include "../common.h"

#include <stdlib.h>
#include <string.h>

VktPipelineBuilder vkt_pipeline_builder_new() {
    VktPipelineBuilder builder;
    memset(&builder, 0, sizeof(VktPipelineBuilder));

    builder.shader_stages_len = 0;
    builder.shader_stages_cap = 2;
    builder.shader_stages = malloc(sizeof(VkPipelineShaderStageCreateInfo) * builder.shader_stages_cap);

    return builder;
}

void vkt_pipeline_builder_set_viewport(VktPipelineBuilder *builder, VkViewport viewport) {
    builder->viewport = viewport;
}

void vkt_pipeline_builder_set_viewport_from_extent(VktPipelineBuilder *builder, VkExtent2D extent) {
    VkViewport viewport;
    viewport.x = 0.f;
    viewport.y = 0.f;
    viewport.width = extent.width;
    viewport.height = extent.height;
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;
    vkt_pipeline_builder_set_viewport(builder, viewport);
}

void vkt_pipeline_builder_set_scissor(VktPipelineBuilder *builder, VkRect2D scissor) {
    builder->scissor = scissor;
}

void vkt_pipeline_builder_set_scissor_from_extent(VktPipelineBuilder *builder, VkExtent2D extent) {
    VkRect2D scissor;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent = extent;
    vkt_pipeline_builder_set_scissor(builder, scissor);
}

void vkt_pipeline_builder_set_pipeline_layout(VktPipelineBuilder *builder, VkPipelineLayout pipeline_layout) {
    builder->pipeline_layout = pipeline_layout;
}

void vkt_pipeline_builder_push_shader_stage(VktPipelineBuilder *builder, VkShaderStageFlagBits stage, VkShaderModule module) {
    VkPipelineShaderStageCreateInfo info;
    memset(&info, 0, sizeof(VkPipelineShaderStageCreateInfo));

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.pNext = NULL;

    info.stage = stage;
    info.module = module;

    info.pName = "main";

    // Reallocate the list if necessary
    if (builder->shader_stages_len >= builder->shader_stages_cap) {
        builder->shader_stages_cap *= 2;
        builder->shader_stages = realloc(builder->shader_stages, sizeof(VkPipelineShaderStageCreateInfo) * builder->shader_stages_cap);
    }

    builder->shader_stages[builder->shader_stages_len] = info;
    builder->shader_stages_len++;
}

void vkt_pipeline_builder_set_vertex_input_state(VktPipelineBuilder *builder) {
    VkPipelineVertexInputStateCreateInfo info;
    memset(&info, 0, sizeof(VkPipelineVertexInputStateCreateInfo));

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    info.pNext = NULL;

    // No vertex bindings and attributes (for now)
    info.vertexBindingDescriptionCount = 0;
    info.vertexAttributeDescriptionCount = 0;

    builder->vertex_input_state = info;
}

void vkt_pipeline_builder_set_input_assembly_state(VktPipelineBuilder *builder, VkPrimitiveTopology topology) {
    VkPipelineInputAssemblyStateCreateInfo info;
    memset(&info, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    info.pNext = NULL;

    info.topology = topology;
    info.primitiveRestartEnable = VK_FALSE;

    builder->input_assembly_state = info;
}

void vkt_pipeline_builder_set_rasterization_state(VktPipelineBuilder *builder, VkPolygonMode polygon_mode) {
    VkPipelineRasterizationStateCreateInfo info;
    memset(&info, 0, sizeof(VkPipelineRasterizationStateCreateInfo));

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.pNext = NULL;

    info.depthClampEnable = VK_FALSE;

    info.rasterizerDiscardEnable = VK_FALSE;

    info.polygonMode = polygon_mode;
    info.lineWidth = 1.0;

    info.cullMode = VK_CULL_MODE_NONE;
    info.frontFace = VK_FRONT_FACE_CLOCKWISE;

    info.depthBiasEnable = VK_FALSE;

    builder->rasterization_state = info;
}

void vkt_pipeline_builder_set_multisampling_state(VktPipelineBuilder *builder, VkSampleCountFlagBits samples) {
    VkPipelineMultisampleStateCreateInfo info;
    memset(&info, 0, sizeof(VkPipelineMultisampleStateCreateInfo));

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    info.pNext = NULL;

    info.sampleShadingEnable = VK_FALSE;
    info.minSampleShading = 1.0;
    info.pSampleMask = NULL;

    info.rasterizationSamples = samples;

    info.alphaToCoverageEnable = VK_FALSE;
    info.alphaToOneEnable = VK_FALSE;

    builder->multisampling_state = info;
}

void vkt_pipeline_builder_set_color_blend_attachment_state(VktPipelineBuilder *builder) {
    VkPipelineColorBlendAttachmentState state;
    memset(&state, 0, sizeof(VkPipelineColorBlendAttachmentState));

    state.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;

    state.blendEnable = VK_FALSE;

    builder->color_blend_attachment_state = state;
}

int vkt_pipeline_builder_build_pipeline(VktVulkanContext *context, VktPipelineBuilder *builder, VkRenderPass pass, VkPipeline *pipeline) {
    // Create viewport state from stored viewport and scissor
    VkPipelineViewportStateCreateInfo viewport_state;
    memset(&viewport_state, 0, sizeof(VkPipelineViewportStateCreateInfo));

    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.pNext = NULL;

    viewport_state.viewportCount = 1;
    viewport_state.pViewports = &builder->viewport;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = &builder->scissor;

    // Create dummy color blend state that won't do any blending
    VkPipelineColorBlendStateCreateInfo color_blend_state;
    memset(&color_blend_state, 0, sizeof(VkPipelineColorBlendStateCreateInfo));

    color_blend_state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_state.pNext = NULL;

    color_blend_state.logicOpEnable = VK_FALSE;
    color_blend_state.logicOp = VK_LOGIC_OP_COPY;
    color_blend_state.attachmentCount = 1;
    color_blend_state.pAttachments = &builder->color_blend_attachment_state;

    // Build the pipeline
    VkGraphicsPipelineCreateInfo pipeline_info;
    memset(&pipeline_info, 0, sizeof(VkGraphicsPipelineCreateInfo));

    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.pNext = NULL;

    pipeline_info.stageCount = builder->shader_stages_len;
    pipeline_info.pStages = builder->shader_stages;

    pipeline_info.pVertexInputState = &builder->vertex_input_state;
    pipeline_info.pInputAssemblyState = &builder->input_assembly_state;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &builder->rasterization_state;
    pipeline_info.pMultisampleState = &builder->multisampling_state;
    pipeline_info.pColorBlendState = &color_blend_state;

    pipeline_info.layout = builder->pipeline_layout;
    pipeline_info.renderPass = pass;
    pipeline_info.subpass = 0;

    pipeline_info.basePipelineHandle = NULL;

    VKT_CHECK(vkCreateGraphicsPipelines(context->logical_device.vk_device, NULL, 1, &pipeline_info, NULL, pipeline));
    return VKT_GENERIC_SUCCESS;
}

void vkt_pipeline_builder_destroy(VktPipelineBuilder *builder) {
    free(builder->shader_stages);
}
