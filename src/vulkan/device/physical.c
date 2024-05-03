#include "physical.h"
#include "queue_families.h"

#include "../common.h"

typedef struct {
    VkPhysicalDevice device;
    int rating;
    int32_t queue_family_index;
} VktPhysicalDeviceRatingResult;

int vkt_rate_physical_device_type(VkPhysicalDeviceType device_type) {
    if (device_type == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)   return 100;
    if (device_type == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) return 50;
    if (device_type == VK_PHYSICAL_DEVICE_TYPE_CPU)            return 10;
    return 0;
}

int vkt_rate_physical_device(
    VkPhysicalDevice device,
    VktFindPhysicalDeviceProps props,
    VktPhysicalDeviceRatingResult *result
) {
    VkPhysicalDeviceProperties device_props;
    vkGetPhysicalDeviceProperties(device, &device_props);

    c_log(C_LOG_SEVERITY_DEBUG, "- %s", device_props.deviceName);
    c_log(C_LOG_SEVERITY_DEBUG, "    - Device Type: %s", string_VkPhysicalDeviceType(device_props.deviceType));

    // Device type
    int rating = 0;
    rating += vkt_rate_physical_device_type(device_props.deviceType);

	// If the device doesn't support the required queues, it should have a bad rating
    int32_t queue_family_idx = -1;
    VKT_CHECK(vkt_find_queue_families_for_device(device, &props.queue_family_bits, &queue_family_idx, 1));
    if (queue_family_idx == -1) {
        rating -= 1000;
    }

    c_log(C_LOG_SEVERITY_DEBUG, "    - Queue Family Index for requested queues: %d", queue_family_idx);

    result->device = device;
    result->rating = rating;
    result->queue_family_index = queue_family_idx;

    return VKT_GENERIC_SUCCESS;
}

int vkt_find_physical_device(VktVulkanInstance *instance, VktFindPhysicalDeviceProps props, VktFindPhysicalDeviceResult *result) {
    memset(result, 0, sizeof(VktFindPhysicalDeviceResult));

    // Get number of physical devices
    uint32_t device_count = 0;
    VKT_CHECK(vkEnumeratePhysicalDevices(instance->vk_instance, &device_count, NULL));

    // Get actual list of physical devices (and clean up if there was an error)
    VkPhysicalDevice *device_list = malloc(sizeof(VkPhysicalDevice) * device_count);
    int enum_result = vkEnumeratePhysicalDevices(instance->vk_instance, &device_count, device_list);
    if (enum_result != VK_SUCCESS) free(device_list);
    VKT_CHECK(enum_result);

    // Get the best physical device
    VktPhysicalDeviceRatingResult best_result;
    memset(&best_result, 0, sizeof(VktPhysicalDeviceRatingResult));

    c_log(C_LOG_SEVERITY_DEBUG, "Found %lu physical device(s):", device_count);
    for (int i = 0; i < device_count; ++i) {
        VkPhysicalDevice curr_device = device_list[i];

        VktPhysicalDeviceRatingResult rating_result;
        VKT_CHECK(vkt_rate_physical_device(curr_device, props, &rating_result));

        if (rating_result.rating > best_result.rating) {
            best_result = rating_result;
        }
    }

    printf("\n");

    // Clean up
    free(device_list);

    // If a device was found, return success and set it in the output
    // If not, return failure
    if (!best_result.device) {
        return VKT_GENERIC_FAILURE;
    }

    result->physical_device = best_result.device;
    result->queue_family_index = best_result.queue_family_index;
    return VKT_GENERIC_SUCCESS;
}
