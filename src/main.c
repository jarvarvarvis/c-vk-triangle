#include "lib/lib_impls.h"
#include "linmath.h/linmath.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <unistd.h>

#include "vulkan/common.h"
#include "vulkan/helper.h"

#include "vulkan/engine/engine.h"
#include "vulkan/engine/commands.h"

#include "vulkan/shader/module.h"
#include "vulkan/pipeline/pipeline.h"
#include "vulkan/pipeline/builder.h"
#include "vulkan/pipeline/layout_builder.h"
#include "vulkan/pipeline/vertex_input.h"

#include "triangle/mesh.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

typedef struct {
    mat4x4 render_matrix;
} VktTrianglePushConstants;

int vkt_create_triangle_pipeline_layout(VktEngine *engine, VkPipelineLayout *layout) {
    VktPipelineLayoutBuilder builder = vkt_pipeline_layout_builder_new();

    vkt_pipeline_layout_builder_push_push_constant(&builder, 0, sizeof(VktTrianglePushConstants), VK_SHADER_STAGE_VERTEX_BIT);

    VKT_CHECK(vkt_pipeline_layout_builder_build_layout(&engine->vk_context, &builder, layout));
    vkt_pipeline_layout_builder_destroy(&builder);
    return VKT_GENERIC_SUCCESS;
}

int vkt_create_triangle_pipeline(VktEngine *engine, VkPipelineLayout layout, VkPipeline *pipeline) {
    VktPipelineBuilder builder = vkt_pipeline_builder_new();

    vkt_pipeline_builder_set_viewport_from_extent(&builder, engine->render_image_extent);
    vkt_pipeline_builder_set_scissor_from_extent(&builder, engine->render_image_extent);

    vkt_pipeline_builder_push_dynamic_state(&builder, VK_DYNAMIC_STATE_VIEWPORT);
    vkt_pipeline_builder_push_dynamic_state(&builder, VK_DYNAMIC_STATE_SCISSOR);

    vkt_pipeline_builder_set_pipeline_layout(&builder, layout);

    VkShaderModule vert_module;
    VKT_CHECK(vkt_load_shader_module_from_file(&engine->vk_context, "resources/shaders/triangle.vert.spv", &vert_module));
    VkShaderModule frag_module;
    VKT_CHECK(vkt_load_shader_module_from_file(&engine->vk_context, "resources/shaders/triangle.frag.spv", &frag_module));

    vkt_pipeline_builder_push_shader_stage(&builder, VK_SHADER_STAGE_VERTEX_BIT, vert_module);
    vkt_pipeline_builder_push_shader_stage(&builder, VK_SHADER_STAGE_FRAGMENT_BIT, frag_module);

    VktVertexInputDescription triangle_input_description = vkt_triangle_mesh_get_input_description();
    vkt_pipeline_builder_set_vertex_input_state_from_description(&builder, &triangle_input_description);

    vkt_pipeline_builder_set_input_assembly_state(&builder, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    vkt_pipeline_builder_set_rasterization_state(&builder, VK_POLYGON_MODE_FILL);
    vkt_pipeline_builder_set_multisampling_state(&builder, VK_SAMPLE_COUNT_1_BIT);
    vkt_pipeline_builder_set_color_blend_attachment_state(&builder);

    int result = vkt_pipeline_builder_build_pipeline(&engine->vk_context, &builder, engine->present_context.main_render_pass, pipeline);
    vkt_pipeline_builder_destroy(&builder);

    vkt_destroy_shader_module(&engine->vk_context, vert_module);
    vkt_destroy_shader_module(&engine->vk_context, frag_module);

    vkt_destroy_vertex_input_description(&triangle_input_description);

    return result;
}

void vkt_on_window_resize(GLFWwindow *window, int width, int height) {
    VktEngine *engine = glfwGetWindowUserPointer(window);
    vkt_engine_on_window_resize(engine, width, height);
}

int main() {
    // Initialize glfw
    if (!glfwInit()) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to initialize glfw!");
        return EXIT_FAILURE;
    }

    // Create the window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "c-vk-triangle", NULL, NULL);
    if (!window) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create glfw window!");
        return EXIT_FAILURE;
    }

    // Create the vulkan engine
    VktEngineCreateProps engine_props;
    engine_props.swapchain_props.desired_present_mode = VK_PRESENT_MODE_FIFO_KHR;

    VktEngine *engine = malloc(sizeof(VktEngine));
    if (vkt_create_engine("c-vk-triangle", window, &engine_props, engine) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create engine!");
        free(engine);
        return EXIT_FAILURE;
    }

    // Set glfw window user pointer to the engine
    // (in an actual engine, you might have a struct that contains more stuff)
    glfwSetWindowUserPointer(window, engine);

    // Set resize callback
    glfwSetWindowSizeCallback(window, vkt_on_window_resize);

    // Create triangle mesh
    VktTriangleMesh triangle_mesh;
    VKT_CHECK(vkt_create_triangle_mesh(engine, &triangle_mesh));

    // Create pipeline layout and pipeline
    VkPipelineLayout triangle_pipeline_layout;
    VKT_CHECK(vkt_create_triangle_pipeline_layout(engine, &triangle_pipeline_layout));
    VkPipeline triangle_pipeline;
    VKT_CHECK(vkt_create_triangle_pipeline(engine, triangle_pipeline_layout, &triangle_pipeline));

    const size_t MAX_FRAMES = 200;
    size_t frame_counter = 0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Recreate the swapchain at the beginning of the frame, when all fences are reset
        // and all semaphores are unsignaled
        VKT_CHECK(vkt_engine_recreate_swapchain_if_necessary(engine));

        VKT_CHECK(vkt_engine_wait_for_last_frame(engine));

        // If we know that the swapchain needs to be recreated after acquiring the next image, continue with the next frame
        uint32_t swapchain_image_index = 0;
        VKT_CHECK(vkt_engine_acquire_next_image(engine, &swapchain_image_index));
        if (engine->need_to_recreate_swapchain) {
            continue;
        }

        VKT_CHECK(vkt_engine_begin_main_command_buffer(engine));
        {
            // Create arguments for begin renderpass command
            VktCmdBeginRenderPassArgs renderpass_args;
            renderpass_args.has_clear_value = true;
            float clear_color[4] = { 0.25, 0.2, 0.25, 1.0 };
            for (int i = 0; i < 4; ++i) {
                renderpass_args.clear_value.color.float32[i] = clear_color[i];
            }
            renderpass_args.swapchain_image_index = swapchain_image_index;

            // Begin and then end the render pass (to perform the image layout transition)
            vkt_engine_cmd_begin_main_render_pass(engine, renderpass_args);
            {
                // Update viewport and scissor (dynamic state)
                VkViewport viewport = vkt_vulkan_helper_viewport_from_extent(engine->render_image_extent);
                vkCmdSetViewport(engine->main_command_buffer, 0, 1, &viewport);

                VkRect2D scissor = vkt_vulkan_helper_rect2d_from_extent(engine->render_image_extent);
                vkCmdSetScissor(engine->main_command_buffer, 0, 1, &scissor);

                vkCmdBindPipeline(engine->main_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, triangle_pipeline);

                VkDeviceSize offset = 0;
                vkCmdBindVertexBuffers(engine->main_command_buffer, 0, 1, &triangle_mesh.vertex_buffer.buffer, &offset);

                // Compute push constants and upload
                VktTrianglePushConstants push_constants;
                float progress = frame_counter / (float)MAX_FRAMES;
                float angle = progress * 2.0 * M_PI;
                float distance = 0.5f;

                vec3 rotate_axis_Z = { 0.0, 0.0, 1.0 };
                quat rotation_quat_Z; quat_rotate(rotation_quat_Z, angle, rotate_axis_Z);
                mat4x4 rotation_Z; mat4x4_from_quat(rotation_Z, rotation_quat_Z);

                mat4x4 rotation; mat4x4_identity(rotation);
                mat4x4_mul(rotation, rotation, rotation_Z);

                mat4x4 translation; mat4x4_translate(translation, distance * sinf(angle), distance * cosf(angle), 0.0);

                mat4x4_identity(push_constants.render_matrix);
                mat4x4_mul(push_constants.render_matrix, translation, rotation);

                vkCmdPushConstants(
                    engine->main_command_buffer,
                    triangle_pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(VktTrianglePushConstants), &push_constants
                );

                // Finally, draw
                vkCmdDraw(engine->main_command_buffer, triangle_mesh.vertices_len, 1, 0, 0);
            }
            vkt_engine_cmd_end_main_render_pass(engine);
        }
        VKT_CHECK(vkt_engine_end_main_command_buffer(engine));
        VKT_CHECK(vkt_engine_submit_main_command_buffer_to_present_queue(engine));

        VKT_CHECK(vkt_engine_present_queue(engine, swapchain_image_index));

        frame_counter++;
        frame_counter %= MAX_FRAMES;
    }

    // Wait on the present queue before cleaning up (so that all commands are finished)
    VKT_CHECK(vkt_engine_wait_on_present_queue(engine));

    // Clean up
    vkt_destroy_triangle_mesh(engine, &triangle_mesh);
    vkt_destroy_pipeline(&engine->vk_context, triangle_pipeline);
    vkt_destroy_pipeline_layout(&engine->vk_context, triangle_pipeline_layout);

    vkt_destroy_engine(engine);
    free(engine);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
