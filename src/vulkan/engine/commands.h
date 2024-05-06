#ifndef VKT_VULKAN_ENGINE_COMMANDS_H
#define VKT_VULKAN_ENGINE_COMMANDS_H

#include "engine.h"

#include <stdbool.h>

typedef struct {
    bool has_clear_value;
    VkClearValue clear_value;
    uint32_t swapchain_image_index;
} VktCmdBeginRenderPassArgs;

void vkt_engine_cmd_begin_main_render_pass(VktEngine *engine, VktCmdBeginRenderPassArgs args);
void vkt_engine_cmd_end_main_render_pass(VktEngine *engine);

#endif /* VKT_VULKAN_ENGINE_COMMANDS_H */
