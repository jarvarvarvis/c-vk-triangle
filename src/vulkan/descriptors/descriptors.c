#include "descriptors.h"

#include "../common.h"

int vkt_allocate_descriptors_from_pool(
    VktVulkanContext *context,
    VkDescriptorPool pool,
    VkDescriptorSetLayout *layouts,
    uint32_t layouts_count,
    VkDescriptorSet *sets
) {
    VkDescriptorSetAllocateInfo alloc_info;
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.pNext = NULL;

    alloc_info.descriptorPool = pool;
    alloc_info.descriptorSetCount = layouts_count;
    alloc_info.pSetLayouts = layouts;

    VKT_CHECK(vkAllocateDescriptorSets(context->logical_device.vk_device, &alloc_info, sets));
    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_descriptor_set_layout(VktVulkanContext *context, VkDescriptorSetLayout layout) {
    vkDestroyDescriptorSetLayout(context->logical_device.vk_device, layout, NULL);
}

void vkt_destroy_descriptor_pool(VktVulkanContext *context, VkDescriptorPool pool) {
    vkDestroyDescriptorPool(context->logical_device.vk_device, pool, NULL);
}
