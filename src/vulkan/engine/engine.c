#include "engine.h"

#include "../common.h"

#include "../context/present_context.h"
#include "../command/pool.h"
#include "../command/buffer.h"

int vkt_create_engine_frame_data(VktEngine *engine, VktEngineFrameData *data) {
    // Create synchronization structures
    if (vkt_create_engine_sync_structures(&engine->vk_context, &data->sync_structures) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create synchronization structures for frame!");
        return VKT_GENERIC_FAILURE;
    }

    // Create command pool
    if (vkt_create_command_pool(&engine->vk_context, &data->main_command_pool) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create main command pool for frame!");
        return VKT_GENERIC_FAILURE;
    }

    // Create main command buffer
    if (vkt_allocate_primary_command_buffers(&engine->vk_context, data->main_command_pool, &data->main_command_buffer, 1) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to allocate main command buffer for frame!");
        return VKT_GENERIC_FAILURE;
    }

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_engine_frame_data(VktEngine *engine, VktEngineFrameData *data) {
    vkt_free_command_buffers(&engine->vk_context, data->main_command_pool, &data->main_command_buffer, 1);
    vkt_destroy_command_pool(&engine->vk_context, data->main_command_pool);

    vkt_destroy_engine_sync_structures(&engine->vk_context, &data->sync_structures);
}


int vkt_create_engine(const char *app_name, GLFWwindow *window, VktEngineCreateProps *props, VktEngine *engine) {
    if (props->frame_overlap == 0) {
        c_log(C_LOG_SEVERITY_ERROR, "Unable to create engine with frame overlap of 0!");
        return VKT_GENERIC_FAILURE;
    }

    memset(engine, 0, sizeof(VktEngine));

    // Create Vulkan context
    if (vkt_create_basic_graphics_context(&engine->vk_context, "c-vk-triangle") != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create Vulkan context!");
        return VKT_GENERIC_FAILURE;
    }

    // Create present context
    if (vkt_create_present_context(&engine->vk_context, &engine->present_context, window) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create present context!");
        return VKT_GENERIC_FAILURE;
    }

    // Create frames
    engine->frames_count = props->frame_overlap;
    engine->current_frame_index = 0;

    engine->frames = malloc(sizeof(VktEngineFrameData) * engine->frames_count);
    for (size_t i = 0; i < engine->frames_count; ++i) {
        VKT_CHECK(vkt_create_engine_frame_data(engine, &engine->frames[i]));
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
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create swapchain images!");
        return VKT_GENERIC_FAILURE;
    }

    // Create depth image
    if (vkt_create_present_context_depth_image(&engine->vk_context, &engine->present_context) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create depth image!");
        return VKT_GENERIC_FAILURE;
    }

    // Create render pass
    if (vkt_create_present_context_render_pass(&engine->vk_context, &engine->present_context) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create render pass!");
        return VKT_GENERIC_FAILURE;
    }

    // Create framebuffers
    if (vkt_create_present_context_framebuffers(&engine->vk_context, &engine->present_context) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create framebuffers!");
        return VKT_GENERIC_FAILURE;
    }

    engine->creation_props = *props;

    return VKT_GENERIC_SUCCESS;
}

VktEngineFrameData *vkt_engine_current_frame_data(VktEngine *engine) {
    return &engine->frames[engine->current_frame_index];
}

void vkt_engine_select_next_frame(VktEngine *engine) {
    engine->current_frame_index = (engine->current_frame_index + 1) % engine->frames_count;
}

int vkt_engine_wait_for_last_frame(VktEngine *engine) {
    // Wait for the render fence before submitting new commands
    VktEngineFrameData *frame = vkt_engine_current_frame_data(engine);
    return vkt_wait_for_sync_structures_render_fence(&engine->vk_context, &frame->sync_structures);
}

int vkt_engine_cleanup_dangerous_semaphore(VktEngine *engine, VkSemaphore semaphore) {
    // Do a dummy submit to the present queue to unsignal the dangerous semaphore.
    // There doesn't seem to be a cleaner way to do this, unfortunately
    VkSubmitInfo submit_info;

    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = NULL;

    VkPipelineStageFlags wait_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit_info.pWaitDstStageMask = &wait_stage_mask;

    submit_info.waitSemaphoreCount = 0;
    submit_info.pWaitSemaphores = NULL;

    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &semaphore;

    submit_info.commandBufferCount = 0;
    submit_info.pCommandBuffers = NULL;

    VKT_CHECK(vkQueueSubmit(engine->vk_context.present_queue, 1, &submit_info, NULL));
    return VKT_GENERIC_SUCCESS;
}

int vkt_engine_acquire_next_image(VktEngine *engine, uint32_t *image_index) {
    VktEngineFrameData *frame = vkt_engine_current_frame_data(engine);
    int vk_acquire_next_result = vkAcquireNextImageKHR(
        engine->vk_context.logical_device.vk_device,
        engine->present_context.swapchain,
        UINT64_MAX,
        frame->sync_structures.present_complete_semaphore,
        NULL,
        image_index
    );

    if (vk_acquire_next_result == VK_SUBOPTIMAL_KHR || vk_acquire_next_result == VK_ERROR_OUT_OF_DATE_KHR) {
        // Clean up the present_complete_semaphore that is now in a dangerous state
        // vkt_engine_cleanup_dangerous_semaphore(engine, engine->sync_structures.present_complete_semaphore);

        engine->need_to_recreate_swapchain = true;
        return VKT_GENERIC_SUCCESS;
    }

    VKT_CHECK(vk_acquire_next_result);
    return VKT_GENERIC_SUCCESS;
}

int vkt_engine_begin_main_command_buffer(VktEngine *engine) {
    VktEngineFrameData *frame = vkt_engine_current_frame_data(engine);
    return vkt_reset_and_begin_command_buffer_recording(
        &engine->vk_context,
        frame->main_command_buffer,
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    );
}

int vkt_engine_end_main_command_buffer(VktEngine *engine) {
    VktEngineFrameData *frame = vkt_engine_current_frame_data(engine);
    VKT_CHECK(vkEndCommandBuffer(frame->main_command_buffer));
    return VKT_GENERIC_SUCCESS;
}

int vkt_engine_submit_main_command_buffer_to_present_queue(VktEngine *engine) {
    VktEngineFrameData *frame = vkt_engine_current_frame_data(engine);

    VkSubmitInfo submit_info;

    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = NULL;

    VkPipelineStageFlags wait_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit_info.pWaitDstStageMask = &wait_stage_mask;

    // Wait on the present semaphore, as that semaphore is signaled once the swapchain is ready
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &frame->sync_structures.present_complete_semaphore;

    // Signal the render semaphore once rendering is finished
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &frame->sync_structures.render_complete_semaphore;

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &frame->main_command_buffer;

    // Submit the main command buffer to the queue and execute it
    // The render fence will block until the graphics commands have finished execution
    VKT_CHECK(vkQueueSubmit(engine->vk_context.present_queue, 1, &submit_info, frame->sync_structures.render_fence));
    return VKT_GENERIC_SUCCESS;
}

int vkt_engine_present_queue(VktEngine *engine, uint32_t swapchain_image_index) {
    VktEngineFrameData *frame = vkt_engine_current_frame_data(engine);

    VkPresentInfoKHR present_info;
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.pNext = NULL;

    present_info.swapchainCount = 1;
    present_info.pSwapchains = &engine->present_context.swapchain;

    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &frame->sync_structures.render_complete_semaphore;

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

    // Destroy synchronization structures for each frame.
    // Destroying and recreating/reallocating the command pool and command buffers is unnecessary
    // because they will be reset on the next frame anyways.
    for (size_t i = 0; i < engine->frames_count; ++i) {
        VktEngineFrameData *frame = &engine->frames[i];
        vkt_destroy_engine_sync_structures(&engine->vk_context, &frame->sync_structures);
    }

    // Set new swapchain in present context
    engine->present_context.swapchain = new_swapchain;

    // Create new stuff
    VKT_CHECK(vkt_create_present_context_depth_image(&engine->vk_context, &engine->present_context));
    VKT_CHECK(vkt_create_present_context_swapchain_images(&engine->vk_context, &engine->present_context));
    VKT_CHECK(vkt_create_present_context_render_pass(&engine->vk_context, &engine->present_context));
    VKT_CHECK(vkt_create_present_context_framebuffers(&engine->vk_context, &engine->present_context));

    // Recreate synchronization structures for each frame
    for (size_t i = 0; i < engine->frames_count; ++i) {
        VktEngineFrameData *frame = &engine->frames[i];
        VKT_CHECK(vkt_create_engine_sync_structures(&engine->vk_context, &frame->sync_structures));
    }

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
    for (size_t i = 0; i < engine->frames_count; ++i) {
        vkt_destroy_engine_frame_data(engine, &engine->frames[i]);
    }
    free(engine->frames);

    vkt_destroy_present_context(&engine->vk_context, &engine->present_context);
    vkt_destroy_context(&engine->vk_context);
}
