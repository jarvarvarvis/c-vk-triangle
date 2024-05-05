#include "engine.h"

#include "../common.h"

#include "../context/present_context.h"
#include "../command/pool.h"
#include "../command/buffer.h"

int vkt_create_engine(const char *app_name, GLFWwindow *window, VktEngineCreateProps *props, VktEngine *engine) {
    memset(engine, 0, sizeof(VktEngine));

    // Create Vulkan context
    if (vkt_create_basic_graphics_context(&engine->vk_context, "c-vk-triangle") != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create Vulkan context!");
        return VKT_GENERIC_FAILURE;
    }

    // Create synchronization structures
    if (vkt_create_engine_sync_structures(&engine->vk_context, &engine->sync_structures) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create synchronization structures for the engine!");
        return VKT_GENERIC_FAILURE;
    }

    // Create present context
    if (vkt_create_present_context(&engine->vk_context, &engine->present_context, window) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create present context!");
        return VKT_GENERIC_FAILURE;
    }

    // Create swapchain
    if (vkt_create_present_context_swapchain(&engine->vk_context, &engine->present_context, props->swapchain_props) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create swapchain for the window surface!");
        return VKT_GENERIC_FAILURE;
    }

    // Create render pass
    if (vkt_create_present_context_render_pass(&engine->vk_context, &engine->present_context) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create render pass!");
        return VKT_GENERIC_FAILURE;
    }

    // Create framebuffers
    if (vkt_create_present_context_framebuffers(&engine->vk_context, &engine->present_context) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create render pass!");
        return VKT_GENERIC_FAILURE;
    }

    // Create command pool
    if (vkt_create_command_pool(&engine->vk_context, &engine->main_command_pool) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create graphics command pool!");
        return VKT_GENERIC_FAILURE;
    }

    // Create test command buffer
    if (vkt_allocate_primary_command_buffers(&engine->vk_context, engine->main_command_pool, &engine->main_command_buffer, 1) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to allocate test command buffer!");
        return VKT_GENERIC_FAILURE;
    }

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_engine(VktEngine *engine) {
    vkt_free_command_buffers(&engine->vk_context, engine->main_command_pool, &engine->main_command_buffer, 1);
    vkt_destroy_command_pool(&engine->vk_context, engine->main_command_pool);

    vkt_destroy_present_context(&engine->vk_context, &engine->present_context);

    vkt_destroy_engine_sync_structures(&engine->vk_context, &engine->sync_structures);

    vkt_destroy_context(&engine->vk_context);
}
