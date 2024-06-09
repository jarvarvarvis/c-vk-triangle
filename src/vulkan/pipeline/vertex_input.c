#include "vertex_input.h"

#include <stdlib.h>

VktVertexInputDescription vkt_create_vertex_input_description() {
    VktVertexInputDescription description;

    LIST_HELPER_INIT_LIST(description, VkVertexInputBindingDescription, bindings, 8);
    LIST_HELPER_INIT_LIST(description, VkVertexInputAttributeDescription, attributes, 8);

    description.flags = 0;

    return description;
}

void vkt_vertex_input_description_push_binding(VktVertexInputDescription *description, VkVertexInputBindingDescription binding) {
    LIST_HELPER_PUSH_ELEMENT(description, VkVertexInputBindingDescription, bindings, binding, 2);
}

void vkt_vertex_input_description_push_attribute(VktVertexInputDescription *description, VkVertexInputAttributeDescription attribute) {
    LIST_HELPER_PUSH_ELEMENT(description, VkVertexInputAttributeDescription, attributes, attribute, 2);
}

void vkt_destroy_vertex_input_description(VktVertexInputDescription *description) {
    free(description->bindings);
    free(description->attributes);
}

