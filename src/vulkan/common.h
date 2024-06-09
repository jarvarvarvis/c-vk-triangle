#ifndef VKT_VULKAN_COMMON_H
#define VKT_VULKAN_COMMON_H

#include <c_utils_collection/concat.h>

#include <vulkan/vk_enum_string_helper.h>

#include <c_log/c_log.h>

#define VKT_GENERIC_SUCCESS 0
#define VKT_GENERIC_FAILURE 1

#define VKT_CHECK(result) \
    VkResult TOKEN_CONCAT(result_, __LINE__) = result;\
    if (TOKEN_CONCAT(result_, __LINE__) != VK_SUCCESS) { \
        c_log(C_LOG_SEVERITY_ERROR, "Vulkan function check failed in '%s' at line %lu: %s", \
              __FILE__, __LINE__, string_VkResult(TOKEN_CONCAT(result_, __LINE__))); \
        return VKT_GENERIC_FAILURE; \
    }

#endif /* VKT_VULKAN_COMMON_H */
