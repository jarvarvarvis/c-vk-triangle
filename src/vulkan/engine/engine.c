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

    engine->need_to_recreate_swapchain = false;
    engine->render_image_extent = engine->present_context.image_size;

    // Create swapchain images
    if (vkt_create_present_context_swapchain_images(&engine->vk_context, &engine->present_context) != VKT_GENERIC_SUCCESS) {
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

    engine->creation_props = *props;

    return VKT_GENERIC_SUCCESS;
}

int vkt_engine_wait_for_last_frame(VktEngine *engine) {
    // Wait for the render fence before submitting new commands
    return vkt_wait_for_sync_structures_render_fence(&engine->vk_context, &engine->sync_structures);
}

int vkt_engine_acquire_next_image(VktEngine *engine, uint32_t *image_index) {
    int vk_acquire_next_result = vkAcquireNextImageKHR(
        engine->vk_context.logical_device.vk_device,
        engine->present_context.swapchain,
        UINT64_MAX,
        engine->sync_structures.present_complete_semaphore,
        NULL,
        image_index
    );

    if (vk_acquire_next_result == VK_SUBOPTIMAL_KHR || vk_acquire_next_result == VK_ERROR_OUT_OF_DATE_KHR) {
        engine->need_to_recreate_swapchain = true;
        return VKT_GENERIC_SUCCESS;
    }

    VKT_CHECK(vk_acquire_next_result);
    return VKT_GENERIC_SUCCESS;
}

int vkt_engine_begin_main_command_buffer(VktEngine *engine) {
    return vkt_reset_and_begin_command_buffer_recording(
        &engine->vk_context,
        engine->main_command_buffer,
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    );
}

int vkt_engine_end_main_command_buffer(VktEngine *engine) {
    VKT_CHECK(vkEndCommandBuffer(engine->main_command_buffer));
    return VKT_GENERIC_SUCCESS;
}

int vkt_engine_submit_main_command_buffer_to_present_queue(VktEngine *engine) {
    VkSubmitInfo submit_info;

    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = NULL;

    VkPipelineStageFlags wait_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit_info.pWaitDstStageMask = &wait_stage_mask;

    // Wait on the present semaphore, as that semaphore is signaled once the swapchain is ready
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &engine->sync_structures.present_complete_semaphore;

    // Signal the render semaphore once rendering is finished
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &engine->sync_structures.render_complete_semaphore;

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &engine->main_command_buffer;

    // Submit the main command buffer to the queue and execute it
    // The render fence will block until the graphics commands have finished execution
    VKT_CHECK(vkQueueSubmit(engine->vk_context.present_queue, 1, &submit_info, engine->sync_structures.render_fence));
    return VKT_GENERIC_SUCCESS;
}

int vkt_engine_present_queue(VktEngine *engine, uint32_t swapchain_image_index) {
    VkPresentInfoKHR present_info;
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.pNext = NULL;

    present_info.swapchainCount = 1;
    present_info.pSwapchains = &engine->present_context.swapchain;

    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &engine->sync_structures.render_complete_semaphore;

    present_info.pImageIndices = &swapchain_image_index;

    present_info.pResults = NULL;

    int queue_present_result = vkQueuePresentKHR(engine->vk_context.present_queue, &present_info);

    // If the swapchain is suboptimal for this image, only recreate it on the next frame.
    // At this point, the present complete semaphore is still in a signaled state and we have to
    // submit to the present queue to unsignal it.
    if (queue_present_result == VK_SUBOPTIMAL_KHR || queue_present_result == VK_ERROR_OUT_OF_DATE_KHR) {
        engine->need_to_recreate_swapchain = true;
        return VKT_GENERIC_SUCCESS;
    }

    VKT_CHECK(queue_present_result);
    return VKT_GENERIC_SUCCESS;
}

void vkt_engine_on_window_resize(VktEngine *engine, int width, int height) {
    vkt_present_context_update_size(&engine->present_context, width, height);

    // Explicitly set need_to_recreate_swapchain to true
    engine->need_to_recreate_swapchain = true;
}

int vkt_engine_recreate_swapchain_if_necessary(VktEngine *engine) {
    if (!engine->need_to_recreate_swapchain) {
        return VKT_GENERIC_SUCCESS;
    }

    // Wait for the device
    vkDeviceWaitIdle(engine->vk_context.logical_device.vk_device);

    // Query latest surface capabilities to get image extent
    vkt_present_context_update_surface_capabilities(&engine->vk_context, &engine->present_context);

    // Set swapchain image extent
    engine->creation_props.swapchain_props.image_extent = vkt_present_context_get_latest_surface_extent(&engine->present_context);

    // Create a new swapchain from the old swapchain
    VkSwapchainKHR new_swapchain;
    VKT_CHECK(vkt_create_swapchain(
        &engine->vk_context,
        engine->present_context.surface,
        &engine->present_context.surface_info,
        engine->creation_props.swapchain_props,
        engine->present_context.swapchain,      // Use the old swapchain for recreation
        &new_swapchain                          // new_swapchain stores the new swapchain
    ));

    // Destroy the old swapchain and dependent structures in the present context
    vkt_destroy_present_context_swapchain_and_dependents(&engine->vk_context, &engine->present_context);

    // Destroy engine synchronization structures
    vkt_destroy_engine_sync_structures(&engine->vk_context, &engine->sync_structures);

    // Set new swapchain in present context
    engine->present_context.swapchain = new_swapchain;

    // Create new stuff
    VKT_CHECK(vkt_create_present_context_swapchain_images(&engine->vk_context, &engine->present_context));
    VKT_CHECK(vkt_create_present_context_render_pass(&engine->vk_context, &engine->present_context));
    VKT_CHECK(vkt_create_present_context_framebuffers(&engine->vk_context, &engine->present_context));

    VKT_CHECK(vkt_create_engine_sync_structures(&engine->vk_context, &engine->sync_structures));

    engine->need_to_recreate_swapchain = false;

    // Keep track of the image extent the swapchain was created with
    engine->render_image_extent = engine->creation_props.swapchain_props.image_extent;

    return VKT_GENERIC_SUCCESS;
}

int vkt_engine_wait_on_present_queue(VktEngine *engine) {
    VKT_CHECK(vkQueueWaitIdle(engine->vk_context.present_queue));
    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_engine(VktEngine *engine) {
    vkt_free_command_buffers(&engine->vk_context, engine->main_command_pool, &engine->main_command_buffer, 1);
    vkt_destroy_command_pool(&engine->vk_context, engine->main_command_pool);

    vkt_destroy_present_context(&engine->vk_context, &engine->present_context);

    vkt_destroy_engine_sync_structures(&engine->vk_context, &engine->sync_structures);

    vkt_destroy_context(&engine->vk_context);
}
