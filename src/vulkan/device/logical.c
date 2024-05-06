#include "logical.h"

#include "../common.h"

#include <string.h>

int vkt_create_logical_device(VktLogicalDevice *device, VktPhysicalDevice physical_device) {
    memset(device, 0, sizeof(VktLogicalDevice));

    // Device Queue
    VkDeviceQueueCreateInfo queue_info;
    memset(&queue_info, 0, sizeof(VkDeviceQueueCreateInfo));

    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info.pNext = NULL;
    queue_info.queueCount = 1;

    float queue_priorities[1] = {0.0};
    queue_info.pQueuePriorities = queue_priorities;

    queue_info.queueFamilyIndex = physical_device.queue_family_index;

    // Device
    VkDeviceCreateInfo device_info;
    memset(&device_info, 0, sizeof(VkDeviceCreateInfo));

    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.pNext = NULL;

    device_info.queueCreateInfoCount = 1;
    device_info.pQueueCreateInfos = &queue_info;

    // Swapchain extension for graphics
    const char *enabled_extension_names[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    device_info.enabledExtensionCount = sizeof(enabled_extension_names) / sizeof(const char *);
    device_info.ppEnabledExtensionNames = enabled_extension_names;

    // No layers
    device_info.enabledLayerCount = 0;
    device_info.ppEnabledLayerNames = NULL;

    device_info.pEnabledFeatures = NULL;

    // Create the device
    VKT_CHECK(vkCreateDevice(physical_device.vk_device, &device_info, NULL, &device->vk_device));
    device->queue_family_index = physical_device.queue_family_index;

    return VKT_GENERIC_SUCCESS;
}

VkQueue vkt_get_logical_device_queue(VktLogicalDevice *device, uint32_t index) {
    VkQueue queue;
    vkGetDeviceQueue(device->vk_device, device->queue_family_index, index, &queue);
    return queue;
}

void vkt_destroy_logical_device(VktLogicalDevice *device) {
    vkDestroyDevice(device->vk_device, NULL);
}
