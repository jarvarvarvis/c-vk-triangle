#include "layout.h"

#include "../common.h"

#include <string.h>

int vkt_create_basic_pipeline_layout(VktVulkanContext *context, VkPipelineLayout *layout) {
    VkPipelineLayoutCreateInfo info;
    memset(&info, 0, sizeof(VkPipelineLayoutCreateInfo));

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.pNext = NULL;

    VKT_CHECK(vkCreatePipelineLayout(context->logical_device.vk_device, &info, NULL, layout));
    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_pipeline_layout(VktVulkanContext *context, VkPipelineLayout layout) {
    vkDestroyPipelineLayout(context->logical_device.vk_device, layout, NULL);
}
