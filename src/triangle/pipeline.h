#ifndef VKT_TRIANGLE_PIPELINE_H
#define VKT_TRIANGLE_PIPELINE_H

#include "../vulkan/engine/engine.h"

typedef struct {
    VkDescriptorSetLayout descriptor_set_layout;
} VktTrianglePipelineLayoutArgs;

int vkt_create_triangle_pipeline_layout(VktEngine *engine, VktTrianglePipelineLayoutArgs *args, VkPipelineLayout *layout);
int vkt_create_triangle_pipeline(VktEngine *engine, VkPipelineLayout layout, VkPipeline *pipeline);

#endif /* VKT_TRIANGLE_PIPELINE_H */
