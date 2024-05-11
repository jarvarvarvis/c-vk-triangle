#ifndef VKT_PIPELINE_VERTEX_INPUT_H
#define VKT_PIPELINE_VERTEX_INPUT_H

#include <vulkan/vulkan.h>

typedef struct {
    VkVertexInputBindingDescription *bindings;
    size_t bindings_len, bindings_cap;

    VkVertexInputAttributeDescription *attributes;
    size_t attributes_len, attributes_cap;

    VkPipelineVertexInputStateCreateFlags flags;
} VktVertexInputDescription;

VktVertexInputDescription vkt_create_vertex_input_description();
void vkt_vertex_input_description_push_binding(VktVertexInputDescription *description, VkVertexInputBindingDescription binding);
void vkt_vertex_input_description_push_attribute(VktVertexInputDescription *description, VkVertexInputAttributeDescription attribute);
void vkt_destroy_vertex_input_description(VktVertexInputDescription *description);

#endif /* VKT_PIPELINE_VERTEX_INPUT_H */
