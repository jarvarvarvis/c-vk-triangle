#include "queue_families.h"

#include "../common.h"

int vkt_find_queue_families_for_device(
    VkPhysicalDevice device,
    VkQueueFlagBits *families_to_find_flags,
    int32_t *families_to_find_indices_out,
    size_t families_to_find_count
) {
    if (families_to_find_count == 0) return VKT_GENERIC_SUCCESS;

    // Validate pointers
    if (!families_to_find_count) return VKT_GENERIC_FAILURE;
    if (!families_to_find_indices_out) return VKT_GENERIC_FAILURE;

    // Query queue families
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);

    VkQueueFamilyProperties queue_families[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    // Find required queue families
    for (int i = 0; i < families_to_find_count; ++i) {
        VkFlags required_flags = families_to_find_flags[i];
        families_to_find_indices_out[i] = -1;

        // Iterate over queue families
        for (int family_index = 0; family_index < queue_family_count; ++family_index) {
            VkQueueFlags queue_flags = queue_families[family_index].queueFlags;
            c_log(C_LOG_SEVERITY_DEBUG, "vkt_find_queue_families_for_device :: queue family %d has flags: %d", i, queue_flags);

            // If the required flags are supported, set family index in output
            if ((queue_flags & required_flags) == required_flags) {
                families_to_find_indices_out[i] = family_index;
            }
        }
    }

    return VKT_GENERIC_SUCCESS;
}
