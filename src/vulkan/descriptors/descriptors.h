#ifndef VKT_VULKAN_DESCRIPTORS_H
#define VKT_VULKAN_DESCRIPTORS_H

#include "../context/context.h"

int vkt_allocate_descriptors_from_pool(
    VktVulkanContext *context,
    VkDescriptorPool pool,
    VkDescriptorSetLayout *layouts,
    uint32_t layouts_count,
    VkDescriptorSet *sets
);

void vkt_destroy_descriptor_set_layout(VktVulkanContext *context, VkDescriptorSetLayout layout);
void vkt_destroy_descriptor_pool(VktVulkanContext *context, VkDescriptorPool pool);

#endif /* VKT_VULKAN_DESCRIPTORS_H */
