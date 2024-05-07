#include "lib/lib_impls.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <unistd.h>

#include "vulkan/common.h"
#include "vulkan/engine/engine.h"
#include "vulkan/engine/commands.h"

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
            vkt_engine_cmd_end_main_render_pass(engine);
        }
        VKT_CHECK(vkt_engine_end_main_command_buffer(engine));
        VKT_CHECK(vkt_engine_submit_main_command_buffer_to_present_queue(engine));

        VKT_CHECK(vkt_engine_present_queue(engine, swapchain_image_index));
    }

    // Clean up
    vkt_destroy_engine(engine);
    free(engine);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
