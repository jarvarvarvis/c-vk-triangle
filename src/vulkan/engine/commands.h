#ifndef VKT_VULKAN_ENGINE_COMMANDS_H
#define VKT_VULKAN_ENGINE_COMMANDS_H

#include "engine.h"

#include <stdbool.h>

typedef struct {
    bool has_color_clear_value;
    bool has_depth_stencil_clear_value;
    VkClearValue color_clear_value;
    VkClearValue depth_stencil_clear_value;

    uint32_t swapchain_image_index;
} VktCmdBeginRenderPassArgs;

VktCmdBeginRenderPassArgs vkt_create_cmd_begin_render_pass_args(uint32_t swapchain_image_index);
void vkt_set_cmd_begin_render_pass_args_clear_value_color(VktCmdBeginRenderPassArgs *args, float color[4]);
void vkt_set_cmd_begin_render_pass_args_clear_value_depth_stencil(VktCmdBeginRenderPassArgs *args, float depth, uint32_t stencil);

void vkt_engine_cmd_begin_main_render_pass(VktEngine *engine, VktCmdBeginRenderPassArgs args);
void vkt_engine_cmd_end_main_render_pass(VktEngine *engine);

#endif /* VKT_VULKAN_ENGINE_COMMANDS_H */
