#include "vertex_input.h"

#include <stdlib.h>

VktVertexInputDescription vkt_create_vertex_input_description() {
    VktVertexInputDescription description;

    VKT_LIST_HELPER_INIT_LIST(VkVertexInputBindingDescription, description, bindings, 8);
    VKT_LIST_HELPER_INIT_LIST(VkVertexInputAttributeDescription, description, attributes, 8);

    description.flags = 0;

    return description;
}

void vkt_vertex_input_description_push_binding(VktVertexInputDescription *description, VkVertexInputBindingDescription binding) {
    VKT_LIST_HELPER_PUSH_ELEMENT(VkVertexInputBindingDescription, description, bindings, 2, binding);
}

void vkt_vertex_input_description_push_attribute(VktVertexInputDescription *description, VkVertexInputAttributeDescription attribute) {
    VKT_LIST_HELPER_PUSH_ELEMENT(VkVertexInputAttributeDescription, description, attributes, 2, attribute);
}

void vkt_destroy_vertex_input_description(VktVertexInputDescription *description) {
    free(description->bindings);
    free(description->attributes);
}

