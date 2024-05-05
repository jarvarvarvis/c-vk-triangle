#ifndef VKT_VULKAN_ENGINE_H
#define VKT_VULKAN_ENGINE_H

#include "../context/context.h"
#include "../context/present_context.h"

typedef struct {
    VktSwapchainCreateProps swapchain_props;
} VktEngineCreateProps;

typedef struct {
    VktVulkanContext vk_context;
    VktPresentContext present_context;

    VkCommandPool main_command_pool;
    VkCommandBuffer main_command_buffer;
} VktEngine;

int vkt_create_engine(const char *app_name, GLFWwindow *window, VktEngineCreateProps *props, VktEngine *engine);
void vkt_destroy_engine(VktEngine *engine);

#endif /* VKT_VULKAN_ENGINE_H */
