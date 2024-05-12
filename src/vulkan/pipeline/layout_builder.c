#include "layout_builder.h"

#include "../common.h"

#include <string.h>

VktPipelineLayoutBuilder vkt_pipeline_layout_builder_new() {
    VktPipelineLayoutBuilder builder;
    VKT_LIST_HELPER_INIT_LIST(VkPushConstantRange, builder, push_constants, 4);
    return builder;
}

void vkt_pipeline_layout_builder_push_push_constant_range(VktPipelineLayoutBuilder *builder, VkPushConstantRange range) {
    VKT_LIST_HELPER_PUSH_ELEMENT(VkPushConstantRange, builder, push_constants, 2, range);
}

void vkt_pipeline_layout_builder_push_push_constant(VktPipelineLayoutBuilder *builder, uint32_t offset, uint32_t size, VkShaderStageFlags shader_stage) {
    VkPushConstantRange range;
    range.offset = offset;
    range.size = size;
    range.stageFlags = shader_stage;

    vkt_pipeline_layout_builder_push_push_constant_range(builder, range);
}

int vkt_pipeline_layout_builder_build_layout(VktVulkanContext *context, VktPipelineLayoutBuilder *builder, VkPipelineLayout *layout) {
    VkPipelineLayoutCreateInfo info;
    memset(&info, 0, sizeof(VkPipelineLayoutCreateInfo));

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.pNext = NULL;

    info.pushConstantRangeCount = builder->push_constants_len;
    info.pPushConstantRanges = builder->push_constants;

    VKT_CHECK(vkCreatePipelineLayout(context->logical_device.vk_device, &info, NULL, layout));
    return VKT_GENERIC_SUCCESS;
}

void vkt_pipeline_layout_builder_destroy(VktPipelineLayoutBuilder *builder) {
    free(builder->push_constants);
}
