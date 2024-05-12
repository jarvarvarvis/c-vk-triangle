#include "mesh.h"

#include "../vulkan/common.h"

VktVertexInputDescription vkt_triangle_mesh_get_input_description() {
    VktVertexInputDescription description = vkt_create_vertex_input_description();

    // 1 vertex buffer binding, with a per-vertex rate
    VkVertexInputBindingDescription main_binding;
    main_binding.binding = 0;
    main_binding.stride = sizeof(VktTriangleVertex);
    main_binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    vkt_vertex_input_description_push_binding(&description, main_binding);

    // Position is stored at location 0
    VkVertexInputAttributeDescription position_attribute;
    position_attribute.binding = 0;
    position_attribute.location = 0;
    position_attribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    position_attribute.offset = offsetof(VktTriangleVertex, position);

    vkt_vertex_input_description_push_attribute(&description, position_attribute);

    // Color is stored at location 1
    VkVertexInputAttributeDescription color_attribute;
    color_attribute.binding = 0;
    color_attribute.location = 1;
    color_attribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    color_attribute.offset = offsetof(VktTriangleVertex, color);

    vkt_vertex_input_description_push_attribute(&description, color_attribute);

    return description;
}

int vkt_create_triangle_mesh(VktEngine *engine, VktTriangleMesh *mesh) {
    memset(mesh, 0, sizeof(VktTriangleMesh));

    mesh->vertices_len = 3;
    size_t vertex_allocation_size = sizeof(VktTriangleVertex) * 3;

    mesh->vertices = malloc(vertex_allocation_size);

    // Vertex position
    memcpy(mesh->vertices[0].position, (vec3) {  0.5,  0.5, 0.0 }, sizeof(float)*3);
    memcpy(mesh->vertices[1].position, (vec3) { -0.5,  0.5, 0.0 }, sizeof(float)*3);
    memcpy(mesh->vertices[2].position, (vec3) {  0.0, -0.5, 0.0 }, sizeof(float)*3);

    // Vertex colors
    memcpy(mesh->vertices[0].color, (vec3) { 1.0, 0.0, 0.0 }, sizeof(float)*3);
    memcpy(mesh->vertices[1].color, (vec3) { 0.0, 1.0, 0.0 }, sizeof(float)*3);
    memcpy(mesh->vertices[2].color, (vec3) { 0.0, 0.0, 1.0 }, sizeof(float)*3);

    // Create vertex buffer and upload data
    VKT_CHECK(vkt_allocate_buffer(engine, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertex_allocation_size, &mesh->vertex_buffer));
    VKT_CHECK(vkt_upload_to_buffer(engine, &mesh->vertex_buffer, mesh->vertices, vertex_allocation_size));

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_triangle_mesh(VktEngine *engine, VktTriangleMesh *mesh) {
    vkt_destroy_buffer(engine, &mesh->vertex_buffer);
    free(mesh->vertices);
}
