#include "push_constants.h"

void vkt_init_triangle_push_data(VktEngine *engine, float fov, vec3 position, vec3 rotation_axis, VktTrianglePushData *data) {
    data->fov = fov;
    vec3_dup(data->position, position);

    vec3_dup(data->rotation_axis, rotation_axis);
    vec3_norm(data->rotation_axis, data->rotation_axis);
}

void vkt_update_triangle_push_data(VktEngine *engine, size_t counter, size_t max_count, VktTrianglePushData *data) {
    // Projection matrix
    float aspect = engine->render_image_extent.width / (float)engine->render_image_extent.height;
    mat4x4_perspective(data->projection, data->fov, aspect, 0.1, 100.0);

    // View matrix
    mat4x4_translate(data->view, data->position[0], data->position[1], data->position[2]);

    // Model matrix
    float progress = counter / (float)max_count;
    float angle = progress * 2.0 * M_PI;

    quat rotation_quat; quat_rotate(rotation_quat, angle, data->rotation_axis);
    mat4x4 rotation_mat; mat4x4_from_quat(rotation_mat, rotation_quat);

    mat4x4 rotation; mat4x4_identity(rotation);
    mat4x4_mul(rotation, rotation, rotation_mat);

    mat4x4_dup(data->model, rotation);
}

void vkt_calculate_triangle_push_constants(VktTrianglePushData *data, VktTrianglePushConstants *push_constants) {
    // Calculate final matrix
    mat4x4 view_model; mat4x4_mul(view_model, data->view, data->model);
    mat4x4_mul(push_constants->render_matrix, data->projection, view_model);
}
