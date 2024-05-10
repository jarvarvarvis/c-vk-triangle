#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1

#define VKT_STRINGIFY(expr) #expr

#define VMA_ASSERT(expr) \
    if (!(expr)) { printf("VMA :: Assertion failed: '%s' at %s:%s \n", \
                          VKT_STRINGIFY(expr), __FILE__, __LINE__); }

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
