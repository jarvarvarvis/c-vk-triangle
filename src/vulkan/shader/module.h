#ifndef VKT_VULKAN_SHADER_LOAD_H
#define VKT_VULKAN_SHADER_LOAD_H

#include <vulkan/vulkan.h>

#include "../context/context.h"

int vkt_load_shader_module_from_file(
    VktVulkanContext *context,
    char *file_path,
    VkShaderModule *module
);

void vkt_destroy_shader_module(VktVulkanContext *ctx, VkShaderModule module);

#endif /* VKT_VULKAN_SHADER_LOAD_H */
