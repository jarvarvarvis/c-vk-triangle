#include "pipeline.h"

void vkt_destroy_pipeline_layout(VktVulkanContext *context, VkPipelineLayout layout) {
    vkDestroyPipelineLayout(context->logical_device.vk_device, layout, NULL);
}

void vkt_destroy_pipeline(VktVulkanContext *context, VkPipeline pipeline) {
    vkDestroyPipeline(context->logical_device.vk_device, pipeline, NULL);
}
