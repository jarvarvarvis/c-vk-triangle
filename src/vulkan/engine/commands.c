#include "commands.h"

#include <c_log/c_log.h>

VktCmdBeginRenderPassArgs vkt_create_cmd_begin_render_pass_args(uint32_t swapchain_image_index) {
    VktCmdBeginRenderPassArgs args;
    memset(&args, 0, sizeof(VktCmdBeginRenderPassArgs));

    args.has_color_clear_value = false;
    args.has_depth_stencil_clear_value = false;
    args.swapchain_image_index = swapchain_image_index;

    return args;
}

void vkt_set_cmd_begin_render_pass_args_clear_value_color(VktCmdBeginRenderPassArgs *args, float color[4]) {
    args->has_color_clear_value = true;
    for (int i = 0; i < 4; ++i) {
        args->color_clear_value.color.float32[i] = color[i];
    }
}

void vkt_set_cmd_begin_render_pass_args_clear_value_depth_stencil(VktCmdBeginRenderPassArgs *args, float depth, uint32_t stencil) {
    args->has_depth_stencil_clear_value = true;
    args->depth_stencil_clear_value.depthStencil.depth = depth;
    args->depth_stencil_clear_value.depthStencil.stencil = stencil;
}

void vkt_engine_cmd_begin_main_render_pass(VktEngine *engine, VktCmdBeginRenderPassArgs args) {
    VkRenderPassBeginInfo render_pass_info;

    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.pNext = NULL;

    render_pass_info.renderPass = engine->present_context.main_render_pass;
    render_pass_info.renderArea.offset.x = 0;
    render_pass_info.renderArea.offset.y = 0;

    // If the window is resized *during* the current iteration of the main loop, the image extent
    // returned from vkt_present_context_get_latest_surface_extent will be *too* up-to-date.
    // That's why we keep track of the image size that the latest swapchain was created with.
    // (This fixes a validation message I got sometimes)

    // VkExtent2D extent = vkt_present_context_get_latest_surface_extent(&engine->present_context);
    VkExtent2D extent = engine->render_image_extent;
    render_pass_info.renderArea.extent.width = extent.width;
    render_pass_info.renderArea.extent.height = extent.height;

    render_pass_info.renderPass = engine->present_context.main_render_pass;
    render_pass_info.framebuffer = engine->present_context.framebuffers.framebuffers[args.swapchain_image_index];

    // Connect clear values (if set)
    size_t clear_value_count = 0;
    VkClearValue clear_values[2];
    memset(clear_values, 0, sizeof(clear_values));

    if (args.has_color_clear_value) {
        clear_values[clear_value_count++] = args.color_clear_value;
    }
    if (args.has_depth_stencil_clear_value) {
        clear_values[clear_value_count++] = args.depth_stencil_clear_value;
    }

    render_pass_info.clearValueCount = clear_value_count;
    render_pass_info.pClearValues = clear_values;

    // Begin the render pass
    vkCmdBeginRenderPass(engine->main_command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
}

void vkt_engine_cmd_end_main_render_pass(VktEngine *engine) {
    vkCmdEndRenderPass(engine->main_command_buffer);
}
