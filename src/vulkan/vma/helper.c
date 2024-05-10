#include "helper.h"

#include "../common.h"

int vkt_vma_helper_create_allocator(VktVulkanContext *context, VmaAllocator *allocator) {
    VmaAllocatorCreateInfo allocator_info;
    memset(&allocator_info, 0, sizeof(VmaAllocatorCreateInfo));

    allocator_info.flags = 0;
    allocator_info.vulkanApiVersion = VK_API_VERSION_1_0;
    allocator_info.physicalDevice = context->physical_device.vk_device;
    allocator_info.device = context->logical_device.vk_device;
    allocator_info.instance = context->instance.vk_instance;

    VmaVulkanFunctions vulkan_functions;
    memset(&vulkan_functions, 0, sizeof(VmaVulkanFunctions));

    vulkan_functions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
    vulkan_functions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;

    allocator_info.pVulkanFunctions = &vulkan_functions;

    VKT_CHECK(vmaCreateAllocator(&allocator_info, allocator));
    return VKT_GENERIC_SUCCESS;
}
