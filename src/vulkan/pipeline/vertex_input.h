#ifndef VKT_PIPELINE_VERTEX_INPUT_H
#define VKT_PIPELINE_VERTEX_INPUT_H

#include <vulkan/vulkan.h>

#include <c_utils_collection/datatypes/list_helper.h>

typedef struct {
    LIST_HELPER_STRUCT_MEMBERS(VkVertexInputBindingDescription, bindings);
    LIST_HELPER_STRUCT_MEMBERS(VkVertexInputAttributeDescription, attributes);

    VkPipelineVertexInputStateCreateFlags flags;
} VktVertexInputDescription;

VktVertexInputDescription vkt_create_vertex_input_description();
void vkt_vertex_input_description_push_binding(VktVertexInputDescription *description, VkVertexInputBindingDescription binding);
void vkt_vertex_input_description_push_attribute(VktVertexInputDescription *description, VkVertexInputAttributeDescription attribute);
void vkt_destroy_vertex_input_description(VktVertexInputDescription *description);

#endif /* VKT_PIPELINE_VERTEX_INPUT_H */
