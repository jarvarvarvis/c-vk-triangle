#ifndef VKT_VULKAN_COMMON_H
#define VKT_VULKAN_COMMON_H

#include "../util/concat.h"

#include <vulkan/vk_enum_string_helper.h>

#include <c_log/c_log.h>

#define VKT_GENERIC_SUCCESS 0
#define VKT_GENERIC_FAILURE 1

#define VKT_CHECK(result) \
    VkResult VKT_CONCAT(result_, __LINE__) = result;\
    if (VKT_CONCAT(result_, __LINE__) != VK_SUCCESS) { \
        c_log(C_LOG_SEVERITY_ERROR, "Vulkan function check failed in '%s' at line %lu: %s", \
              __FILE__, __LINE__, string_VkResult(VKT_CONCAT(result_, __LINE__))); \
        return VKT_GENERIC_FAILURE; \
    }


static const char *const VKT_ENABLED_LAYER_NAMES[] = {
    // Enable VK_LAYER_KHRONOS_validation in debug build mode
#ifdef BUILD_MODE_DEBUG
    "VK_LAYER_KHRONOS_validation"
#endif
};

static const char *const VKT_EXTRA_ENABLED_EXTENSIONS[] = {
    // Enable VK_EXT_debug_utils in debug build mode
#ifdef BUILD_MODE_DEBUG
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
};

#endif /* VKT_VULKAN_COMMON_H */
