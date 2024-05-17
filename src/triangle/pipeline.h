#ifndef VKT_TRIANGLE_PIPELINE_H
#define VKT_TRIANGLE_PIPELINE_H

#include "../vulkan/engine/engine.h"

int vkt_create_triangle_pipeline_layout(VktEngine *engine, VkPipelineLayout *layout);
int vkt_create_triangle_pipeline(VktEngine *engine, VkPipelineLayout layout, VkPipeline *pipeline);

#endif /* VKT_TRIANGLE_PIPELINE_H */
