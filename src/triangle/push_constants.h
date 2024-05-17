#ifndef VKT_TRIANGLE_PUSH_CONSTANTS_H
#define VKT_TRIANGLE_PUSH_CONSTANTS_H

#include "../vulkan/engine/engine.h"

#include <linmath.h/linmath.h>

typedef struct {
    mat4x4 render_matrix;
} VktTrianglePushConstants;

typedef struct {
    float fov;
    vec3 position;
    vec3 rotation_axis;

    mat4x4 model;
    mat4x4 view;
    mat4x4 projection;
} VktTrianglePushData;

void vkt_init_triangle_push_data(VktEngine *engine, float fov, vec3 position, vec3 rotation_axis, VktTrianglePushData *data);

void vkt_update_triangle_push_data(VktEngine *engine, size_t counter, size_t max_count, VktTrianglePushData *data);
void vkt_calculate_triangle_push_constants(VktTrianglePushData *data, VktTrianglePushConstants *push_constants);

#endif /* VKT_TRIANGLE_PUSH_CONSTANTS_H */
