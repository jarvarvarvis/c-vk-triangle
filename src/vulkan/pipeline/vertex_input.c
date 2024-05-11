#include "vertex_input.h"

#include <stdlib.h>

VktVertexInputDescription vkt_create_vertex_input_description() {
    VktVertexInputDescription description;

    description.bindings_len = 0;
    description.bindings_cap = 8;
    description.bindings = malloc(sizeof(VkVertexInputBindingDescription) * description.bindings_cap);

    description.attributes_len = 0;
    description.attributes_cap = 8;
    description.attributes = malloc(sizeof(VkVertexInputAttributeDescription) * description.attributes_cap);

    description.flags = 0;

    return description;
}

void vkt_vertex_input_description_push_binding(VktVertexInputDescription *description, VkVertexInputBindingDescription binding) {
    if (description->bindings_len >= description->bindings_cap) {
        description->bindings_cap *= 2;
        description->bindings = realloc(description->bindings, sizeof(VkVertexInputBindingDescription) * description->bindings_cap);
    }

    description->bindings[description->bindings_len] = binding;
    description->bindings_len++;
}

void vkt_vertex_input_description_push_attribute(VktVertexInputDescription *description, VkVertexInputAttributeDescription attribute) {
    if (description->attributes_len >= description->attributes_cap) {
        description->attributes_cap *= 2;
        description->attributes = realloc(description->attributes, sizeof(VkVertexInputAttributeDescription) * description->attributes_cap);
    }

    description->attributes[description->attributes_len] = attribute;
    description->attributes_len++;
}

void vkt_destroy_vertex_input_description(VktVertexInputDescription *description) {
    free(description->bindings);
    free(description->attributes);
}

