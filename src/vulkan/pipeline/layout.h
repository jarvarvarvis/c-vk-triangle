#ifndef VKT_VULKAN_PIPELINE_LAYOUT_H
#define VKT_VULKAN_PIPELINE_LAYOUT_H

#include "../context/context.h"

int vkt_create_basic_pipeline_layout(VktVulkanContext *context, VkPipelineLayout *layout);

void vkt_destroy_pipeline_layout(VktVulkanContext *context, VkPipelineLayout layout);

#endif /* VKT_VULKAN_PIPELINE_LAYOUT_H */
