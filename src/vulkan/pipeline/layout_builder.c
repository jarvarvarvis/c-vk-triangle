#include "layout_builder.h"

#include "../common.h"

#include <string.h>

VktPipelineLayoutBuilder vkt_pipeline_layout_builder_new() {
    VktPipelineLayoutBuilder builder;
    VKT_LIST_HELPER_INIT_LIST(builder, VkPushConstantRange, push_constants, 4);
    VKT_LIST_HELPER_INIT_LIST(builder, VkDescriptorSetLayout, descriptor_set_layouts, 4);
    return builder;
}

void vkt_pipeline_layout_builder_push_push_constant_range(VktPipelineLayoutBuilder *builder, VkPushConstantRange range) {
    VKT_LIST_HELPER_PUSH_ELEMENT(builder, VkPushConstantRange, push_constants, range, 2);
}

void vkt_pipeline_layout_builder_push_push_constant(VktPipelineLayoutBuilder *builder, uint32_t offset, uint32_t size, VkShaderStageFlags shader_stage) {
    VkPushConstantRange range;
    range.offset = offset;
    range.size = size;
    range.stageFlags = shader_stage;

    vkt_pipeline_layout_builder_push_push_constant_range(builder, range);
}

void vkt_pipeline_layout_builder_push_descriptor_set_layout(VktPipelineLayoutBuilder *builder, VkDescriptorSetLayout layout) {
    VKT_LIST_HELPER_PUSH_ELEMENT(builder, VkDescriptorSetLayout, descriptor_set_layouts, layout, 2);
}

int vkt_pipeline_layout_builder_build_layout(VktVulkanContext *context, VktPipelineLayoutBuilder *builder, VkPipelineLayout *layout) {
    VkPipelineLayoutCreateInfo info;
    memset(&info, 0, sizeof(VkPipelineLayoutCreateInfo));

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.pNext = NULL;

    // Set push constant ranges if any were added
    if (builder->push_constants_len > 0) {
        info.pushConstantRangeCount = builder->push_constants_len;
        info.pPushConstantRanges = builder->push_constants;
    }

    // Set descriptor set layouts if any were added
    if (builder->descriptor_set_layouts_len > 0) {
        info.setLayoutCount = builder->descriptor_set_layouts_len;
        info.pSetLayouts = builder->descriptor_set_layouts;
    }

    VKT_CHECK(vkCreatePipelineLayout(context->logical_device.vk_device, &info, NULL, layout));
    return VKT_GENERIC_SUCCESS;
}

void vkt_pipeline_layout_builder_destroy(VktPipelineLayoutBuilder *builder) {
    free(builder->push_constants);
    free(builder->descriptor_set_layouts);
}
