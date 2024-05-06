#include "commands.h"

#include <c_log/c_log.h>

void vkt_engine_cmd_begin_main_render_pass(VktEngine *engine, VktCmdBeginRenderPassArgs args) {
    VkRenderPassBeginInfo render_pass_info;

    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.pNext = NULL;

    render_pass_info.renderPass = engine->present_context.main_render_pass;
    render_pass_info.renderArea.offset.x = 0;
    render_pass_info.renderArea.offset.y = 0;

    VkExtent2D extent = vkt_present_context_get_swapchain_extent(&engine->present_context);
    render_pass_info.renderArea.extent.width = extent.width;
    render_pass_info.renderArea.extent.height = extent.height;

    render_pass_info.renderPass = engine->present_context.main_render_pass;
    render_pass_info.framebuffer = engine->present_context.framebuffers.framebuffers[args.swapchain_image_index];

    // Connect clear value (if set)
    if (args.has_clear_value) {
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &args.clear_value;
    } else {
        render_pass_info.clearValueCount = 0;
        render_pass_info.pClearValues = NULL;
    }

    vkCmdBeginRenderPass(engine->main_command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
}

void vkt_engine_cmd_end_main_render_pass(VktEngine *engine) {
    vkCmdEndRenderPass(engine->main_command_buffer);
}
