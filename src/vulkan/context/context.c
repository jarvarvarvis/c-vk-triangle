#include "context.h"

#include "../common.h"

#include <string.h>
#include <c_log/c_log.h>

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

int vkt_create_basic_graphics_context(VktVulkanContext *context, char *app_name) {
    memset(context, 0, sizeof(VktVulkanContext));

    // Create instance
    if (vkt_create_vulkan_instance(&context->instance, app_name) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create Vulkan instance!");
        return VKT_GENERIC_FAILURE;
    }

    // Physical device discovery
    VktFindPhysicalDeviceProps find_phys_device_props;
    find_phys_device_props.queue_family_bits = VK_QUEUE_GRAPHICS_BIT;
    if (vkt_find_physical_device(&context->instance, find_phys_device_props, &context->physical_device) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Unable to find physical device!");
        return VKT_GENERIC_FAILURE;
    }

    // Create logical device
    if (vkt_create_logical_device(&context->logical_device, context->physical_device) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Unable to create logical device!");
        return VKT_GENERIC_FAILURE;
    }

    // Create VMA allocator
    if (vkt_vma_helper_create_allocator(context, &context->allocator) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create VMA allocator!");
        return VKT_GENERIC_FAILURE;
    }

    // Get the present queue
    context->present_queue = vkt_get_logical_device_queue(&context->logical_device, 0);

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_context(VktVulkanContext *context) {
    vmaDestroyAllocator(context->allocator);

    vkt_destroy_logical_device(&context->logical_device);
    vkt_destroy_vulkan_instance(&context->instance);
}
