#include "layout_builder.h"

#include "../common.h"

#include <string.h>

VktPipelineLayoutBuilder vkt_pipeline_layout_builder_new() {
    VktPipelineLayoutBuilder builder;
    return builder;
}

int vkt_pipeline_layout_builder_build_layout(VktVulkanContext *context, VktPipelineLayoutBuilder *builder, VkPipelineLayout *layout) {
    VkPipelineLayoutCreateInfo info;
    memset(&info, 0, sizeof(VkPipelineLayoutCreateInfo));

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.pNext = NULL;

    VKT_CHECK(vkCreatePipelineLayout(context->logical_device.vk_device, &info, NULL, layout));
    return VKT_GENERIC_SUCCESS;
}

void vkt_pipeline_layout_builder_destroy(VktPipelineLayoutBuilder *builder) {}
