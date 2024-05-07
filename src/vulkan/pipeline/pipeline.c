#include "pipeline.h"

void vkt_destroy_pipeline(VktVulkanContext *context, VkPipeline pipeline) {
    vkDestroyPipeline(context->logical_device.vk_device, pipeline, NULL);
}
