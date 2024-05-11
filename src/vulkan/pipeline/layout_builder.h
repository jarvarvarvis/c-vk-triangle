#ifndef VKT_VULKAN_PIPELINE_LAYOUT_BUILDER_H
#define VKT_VULKAN_PIPELINE_LAYOUT_BUILDER_H

#include "../context/context.h"

typedef struct {

} VktPipelineLayoutBuilder;

VktPipelineLayoutBuilder vkt_pipeline_layout_builder_new();

int vkt_pipeline_layout_builder_build_layout(VktVulkanContext *context, VktPipelineLayoutBuilder *builder, VkPipelineLayout *layout);

void vkt_pipeline_layout_builder_destroy(VktPipelineLayoutBuilder *builder);

#endif /* VKT_VULKAN_PIPELINE_LAYOUT_BUILDER_H */
