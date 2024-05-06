#ifndef VKT_VULKAN_ENGINE_H
#define VKT_VULKAN_ENGINE_H

#include "../context/context.h"
#include "../context/present_context.h"

#include "sync.h"

typedef struct {
    VktSwapchainCreateProps swapchain_props;
} VktEngineCreateProps;

typedef struct {
    VktVulkanContext vk_context;
    VktPresentContext present_context;

    VktEngineSyncStructures sync_structures;

    VkCommandPool main_command_pool;
    VkCommandBuffer main_command_buffer;
} VktEngine;

int vkt_create_engine(const char *app_name, GLFWwindow *window, VktEngineCreateProps *props, VktEngine *engine);

int vkt_engine_wait_for_last_frame(VktEngine *engine);
int vkt_engine_acquire_next_image(VktEngine *engine, uint32_t *image_index);
int vkt_engine_begin_main_command_buffer(VktEngine *engine);
int vkt_engine_end_main_command_buffer(VktEngine *engine);
int vkt_engine_submit_main_command_buffer_to_present_queue(VktEngine *engine);
int vkt_engine_present_queue(VktEngine *engine, uint32_t swapchain_image_index);

void vkt_destroy_engine(VktEngine *engine);

#endif /* VKT_VULKAN_ENGINE_H */
