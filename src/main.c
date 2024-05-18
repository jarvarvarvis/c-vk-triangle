#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <unistd.h>

#include "util/math.h"

#include "vulkan/common.h"
#include "vulkan/helper.h"

#include "vulkan/engine/engine.h"
#include "vulkan/engine/commands.h"

#include "vulkan/pipeline/pipeline.h"

#include "triangle/descriptors.h"
#include "triangle/mesh.h"
#include "triangle/pipeline.h"
#include "triangle/push_constants.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

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
    engine_props.frame_overlap = 2;
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

    // Create triangle descriptors, one descriptor set for each frame
    VktTriangleDescriptors triangle_descriptors;
    VKT_CHECK(vkt_create_triangle_descriptors(engine, engine_props.frame_overlap, &triangle_descriptors));

    // Create pipeline layout and pipeline
    VkPipelineLayout triangle_pipeline_layout;

    VktTrianglePipelineLayoutArgs triangle_pipeline_args;
    triangle_pipeline_args.descriptor_set_layout = triangle_descriptors.set_layout;
    VKT_CHECK(vkt_create_triangle_pipeline_layout(engine, &triangle_pipeline_args, &triangle_pipeline_layout));

    VkPipeline triangle_pipeline;
    VKT_CHECK(vkt_create_triangle_pipeline(engine, triangle_pipeline_layout, &triangle_pipeline));

    // Counters for update logic
    const size_t MAX_UPDATE_COUNT = 200;
    size_t update_counter = 0;

    // Create triangle push constant data
    VktTrianglePushData triangle_push_data;
    vkt_init_triangle_push_data(engine, vkt_math_degrees_to_radians(45.f), (vec3) {0.0, 0.0, -3.0}, (vec3) {1.0, 1.0, 1.0}, &triangle_push_data);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        VktEngineFrameData *frame_data = vkt_engine_current_frame_data(engine);

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
            VkCommandBuffer main_command_buffer = frame_data->main_command_buffer;

            // Create arguments for begin renderpass command
            VktCmdBeginRenderPassArgs renderpass_args = vkt_create_cmd_begin_render_pass_args(swapchain_image_index);
            vkt_set_cmd_begin_render_pass_args_clear_value_color(&renderpass_args, (float[4]) {0.25, 0.15, 0.35, 1.0});
            vkt_set_cmd_begin_render_pass_args_clear_value_depth_stencil(&renderpass_args, 1.0, 0);

            // Begin and then end the render pass (to perform the image layout transition)
            vkt_engine_cmd_begin_main_render_pass(engine, renderpass_args);
            {
                // Update viewport and scissor (dynamic state)
                VkViewport viewport = vkt_vulkan_helper_viewport_from_extent(engine->render_image_extent);
                vkCmdSetViewport(main_command_buffer, 0, 1, &viewport);

                VkRect2D scissor = vkt_vulkan_helper_rect2d_from_extent(engine->render_image_extent);
                vkCmdSetScissor(main_command_buffer, 0, 1, &scissor);

                vkCmdBindPipeline(main_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, triangle_pipeline);

                VkDeviceSize offset = 0;
                vkCmdBindVertexBuffers(main_command_buffer, 0, 1, &triangle_mesh.vertex_buffer.buffer, &offset);

                // For the current frame: Upload descriptor data to the buffer and bind the descriptor set
                VktTriangleGlobalUniformData uniform_data;
                mat4x4_identity(uniform_data.mvp);
                mat4x4_translate(uniform_data.mvp, 0.0, 1.0, 0.0);

                vkt_triangle_descriptors_upload_global_data(engine, &triangle_descriptors, engine->current_frame_index, &uniform_data);
                vkt_triangle_descriptors_bind_set(engine, triangle_pipeline_layout, &triangle_descriptors, engine->current_frame_index);

                // Update push data
                vkt_update_triangle_push_data(engine, update_counter, MAX_UPDATE_COUNT, &triangle_push_data);

                VktTrianglePushConstants triangle_push_constants;
                vkt_calculate_triangle_push_constants(&triangle_push_data, &triangle_push_constants);

                vkCmdPushConstants(
                    main_command_buffer,
                    triangle_pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(VktTrianglePushConstants), &triangle_push_constants
                );

                // Finally, draw
                vkCmdDraw(main_command_buffer, triangle_mesh.vertices_len, 1, 0, 0);

                // Translate model matrix in place and recalculate push constants
                mat4x4_translate_in_place(triangle_push_data.model, 0.0, 0.0, -0.5);
                vkt_calculate_triangle_push_constants(&triangle_push_data, &triangle_push_constants);

                vkCmdPushConstants(
                    main_command_buffer,
                    triangle_pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(VktTrianglePushConstants), &triangle_push_constants
                );

                // Draw again
                vkCmdDraw(main_command_buffer, triangle_mesh.vertices_len, 1, 0, 0);
            }
            vkt_engine_cmd_end_main_render_pass(engine);
        }
        VKT_CHECK(vkt_engine_end_main_command_buffer(engine));
        VKT_CHECK(vkt_engine_submit_main_command_buffer_to_present_queue(engine));

        VKT_CHECK(vkt_engine_present_queue(engine, swapchain_image_index));

        vkt_engine_select_next_frame(engine);

        update_counter++;
        update_counter %= MAX_UPDATE_COUNT;
    }

    // Wait on the present queue before cleaning up (so that all commands are finished)
    VKT_CHECK(vkt_engine_wait_on_present_queue(engine));

    // Clean up
    vkt_destroy_triangle_descriptors(engine, &triangle_descriptors);
    vkt_destroy_triangle_mesh(engine, &triangle_mesh);

    vkt_destroy_pipeline(&engine->vk_context, triangle_pipeline);
    vkt_destroy_pipeline_layout(&engine->vk_context, triangle_pipeline_layout);

    vkt_destroy_engine(engine);
    free(engine);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
