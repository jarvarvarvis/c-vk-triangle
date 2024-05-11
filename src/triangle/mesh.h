#ifndef VKT_TRIANGLE_MESH_H
#define VKT_TRIANGLE_MESH_H

#include <linmath.h/linmath.h>

#include "../vulkan/pipeline/vertex_input.h"

#include "../vulkan/buffer/buffer.h"
#include "../vulkan/engine/engine.h"

typedef struct {
    vec3 position;
    vec3 color;
} VktTriangleVertex;

typedef struct {
    VktTriangleVertex *vertices;
    size_t vertices_len;
    VktAllocatedBuffer vertex_buffer;
} VktTriangleMesh;

VktVertexInputDescription vkt_triangle_mesh_get_input_description();

int vkt_create_triangle_mesh(VktEngine *engine, VktTriangleMesh *mesh);
void vkt_destroy_triangle_mesh(VktEngine *engine, VktTriangleMesh *mesh);

#endif /* VKT_TRIANGLE_MESH_H */
