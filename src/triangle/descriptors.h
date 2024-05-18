#ifndef VKT_TRIANGLE_DESCRIPTORS_H
#define VKT_TRIANGLE_DESCRIPTORS_H

#include <linmath.h/linmath.h>

#include "../vulkan/buffer/buffer.h"
#include "../vulkan/engine/engine.h"

typedef struct {
    mat4x4 mvp;
} VktTriangleGlobalUniformData;

typedef struct {
    VkDescriptorSet set;
    VktAllocatedBuffer buffer;
} VktTriangleUniformDescriptors;

typedef struct {
    VkDescriptorSetLayout set_layout;
    VkDescriptorPool descriptor_pool;

    VktTriangleUniformDescriptors *uniform_descriptors;
    size_t uniform_descriptors_count;
} VktTriangleDescriptors;

int vkt_create_triangle_descriptors(VktEngine *engine, size_t uniform_sets, VktTriangleDescriptors *descriptors);

void vkt_triangle_descriptors_upload_global_data(VktEngine *engine, VktTriangleDescriptors *descriptors, size_t uniforms_idx, VktTriangleGlobalUniformData *uniform_data);
void vkt_triangle_descriptors_bind_set(VktEngine *engine, VkPipelineLayout pipeline_layout, VktTriangleDescriptors *descriptors, size_t uniforms_idx);

void vkt_destroy_triangle_descriptors(VktEngine *engine, VktTriangleDescriptors *descriptors);

#endif /* VKT_TRIANGLE_DESCRIPTORS_H */
