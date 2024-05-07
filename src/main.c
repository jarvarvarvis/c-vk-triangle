#include "lib/lib_impls.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <unistd.h>

#include "vulkan/common.h"
#include "vulkan/helper.h"

#include "vulkan/engine/engine.h"
#include "vulkan/engine/commands.h"

#include "vulkan/shaders/module.h"
#include "vulkan/pipeline/builder.h"
#include "vulkan/pipeline/layout.h"
#include "vulkan/pipeline/pipeline.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

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

    vkt_pipeline_builder_set_vertex_input_state(&builder);
    vkt_pipeline_builder_set_input_assembly_state(&builder, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    vkt_pipeline_builder_set_rasterization_state(&builder, VK_POLYGON_MODE_FILL);
    vkt_pipeline_builder_set_multisampling_state(&builder, VK_SAMPLE_COUNT_1_BIT);
    vkt_pipeline_builder_set_color_blend_attachment_state(&builder);

    int result = vkt_pipeline_builder_build_pipeline(&engine->vk_context, &builder, engine->present_context.main_render_pass, pipeline);
    vkt_pipeline_builder_destroy(&builder);

    vkt_destroy_shader_module(&engine->vk_context, vert_module);
    vkt_destroy_shader_module(&engine->vk_context, frag_module);

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

    // Create pipeline layout and pipeline
    VkPipelineLayout triangle_pipeline_layout;
    VKT_CHECK(vkt_create_basic_pipeline_layout(&engine->vk_context, &triangle_pipeline_layout));
    VkPipeline triangle_pipeline;
    VKT_CHECK(vkt_create_triangle_pipeline(engine, triangle_pipeline_layout, &triangle_pipeline));

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Recreate the swapchain at the beginning of the frame, when all fences are reset
        // and all semaphores are unsignaled
        VKT_CHECK(vkt_engine_recreate_swapchain_if_necessary(engine));

        VKT_CHECK(vkt_engine_wait_for_last_frame(engine));

        uint32_t swapchain_image_index = 0;
        VKT_CHECK(vkt_engine_acquire_next_image(engine, &swapchain_image_index));

        VKT_CHECK(vkt_engine_begin_main_command_buffer(engine));
        {
            // Create arguments for begin renderpass command
            VktCmdBeginRenderPassArgs renderpass_args;
            renderpass_args.has_clear_value = true;
            float clear_color[4] = { 0.0, 0.0, 1.0, 1.0 };
            for (int i = 0; i < 4; ++i) {
                renderpass_args.clear_value.color.float32[i] = clear_color[i];
            }
            renderpass_args.swapchain_image_index = swapchain_image_index;

            // Begin and then end the render pass (to perform the image layout transition)
            vkt_engine_cmd_begin_main_render_pass(engine, renderpass_args);
            {
                // Update viewport and scissor (dynamic state)
                VkViewport viewport = vkt_helper_viewport_from_extent(engine->render_image_extent);
                vkCmdSetViewport(engine->main_command_buffer, 0, 1, &viewport);

                VkRect2D scissor = vkt_helper_rect2d_from_extent(engine->render_image_extent);
                vkCmdSetScissor(engine->main_command_buffer, 0, 1, &scissor);

                // Render using the triangle pipeline
                vkCmdBindPipeline(engine->main_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, triangle_pipeline);
                vkCmdDraw(engine->main_command_buffer, 3, 1, 0, 0);
            }
            vkt_engine_cmd_end_main_render_pass(engine);
        }
        VKT_CHECK(vkt_engine_end_main_command_buffer(engine));
        VKT_CHECK(vkt_engine_submit_main_command_buffer_to_present_queue(engine));

        VKT_CHECK(vkt_engine_present_queue(engine, swapchain_image_index));
    }

    // Wait on the present queue before cleaning up (so that all commands are finished)
    VKT_CHECK(vkt_engine_wait_on_present_queue(engine));

    // Clean up
    vkt_destroy_pipeline(&engine->vk_context, triangle_pipeline);
    vkt_destroy_pipeline_layout(&engine->vk_context, triangle_pipeline_layout);
    vkt_destroy_engine(engine);
    free(engine);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
