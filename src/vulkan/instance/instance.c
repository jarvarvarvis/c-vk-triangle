#include "instance.h"

#ifdef BUILD_MODE_DEBUG
#include "debug.h"
#endif

#include "../common.h"

#include "../../util/string_list.h"

#include <stdint.h>

#include <c_log/c_log.h>

VkApplicationInfo vkt_create_basic_application_info(GLFWwindow *window) {
    VkApplicationInfo app_info;
    memset(&app_info, 0, sizeof(VkApplicationInfo));

    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = NULL;

    const char *title = glfwGetWindowTitle(window);
    app_info.pApplicationName = title;
    app_info.applicationVersion = 0;
    app_info.pEngineName = title;
    app_info.engineVersion = 0;

    app_info.apiVersion = VK_API_VERSION_1_0;

    return app_info;
}

VkInstanceCreateInfo vkt_create_instance_create_info(VkApplicationInfo *app_info, StringList *extension_list) {
    VkInstanceCreateInfo inst_create_info;
    memset(&inst_create_info, 0, sizeof(VkInstanceCreateInfo));

    inst_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_create_info.pNext = NULL;
    inst_create_info.flags = 0;

    // Set application info
    inst_create_info.pApplicationInfo = app_info;

    // Enable layers specified in VKT_ENABLED_LAYER_NAMES
    inst_create_info.enabledLayerCount = sizeof(VKT_INSTANCE_ENABLED_LAYER_NAMES) / sizeof(char *);
    inst_create_info.ppEnabledLayerNames = VKT_INSTANCE_ENABLED_LAYER_NAMES;

    // Log enabled extensions
    c_log(C_LOG_SEVERITY_DEBUG, "Enabled Vulkan extensions:");
    for (size_t i = 0; i < extension_list->len; ++i) {
        c_log(C_LOG_SEVERITY_DEBUG, "- %s", extension_list->strings[i]);
    }
    printf("\n");

    // Set extensions
    inst_create_info.enabledExtensionCount = extension_list->len;
    inst_create_info.ppEnabledExtensionNames = (const char *const *) extension_list->strings;

    return inst_create_info;
}

int vkt_create_vulkan_instance(VktVulkanInstance *instance, GLFWwindow *window) {
    memset(instance, 0, sizeof(VktVulkanInstance));

    // Get instance extensions
    uint32_t extension_count = 0;
    const char **extensions = glfwGetRequiredInstanceExtensions(&extension_count);
    if (!extensions) {
        c_log(C_LOG_SEVERITY_ERROR, "Unable to get required Vulkan instance extensions for glfw - Vulkan may not be supported by your device");
        return VKT_GENERIC_FAILURE;
    }

    StringList extension_list = string_list_new();

    // Add instance extensions to the list
    for (size_t i = 0; i < extension_count; ++i) {
        string_list_push(&extension_list, (char *) extensions[i]);
    }

    // Add extra extensions to the list
    size_t extra_extensions_count = sizeof(VKT_INSTANCE_EXTRA_EXTENSION_NAMES) / sizeof(char *);
    for (size_t i = 0; i < extra_extensions_count; ++i) {
        string_list_push(&extension_list, (char *) VKT_INSTANCE_EXTRA_EXTENSION_NAMES[i]);
    }

    // Create application info and instance create info
    VkApplicationInfo app_info = vkt_create_basic_application_info(window);
    VkInstanceCreateInfo inst_create_info = vkt_create_instance_create_info(&app_info, &extension_list);

    // Create the instance, delete extension list before VKT_CHECK
    int vk_create_inst_result = vkCreateInstance(&inst_create_info, NULL, &instance->vk_instance);
    string_list_delete(&extension_list);

    VKT_CHECK(vk_create_inst_result);

    // Create debug messenger
#ifdef BUILD_MODE_DEBUG
    int vk_create_messenger_result = vkt_debug_make_messenger(instance->vk_instance, &instance->debug_messenger);
    VKT_CHECK(vk_create_messenger_result);
#endif

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_vulkan_instance(VktVulkanInstance *instance) {
    // Destroy debug messenger before instance
#ifdef BUILD_MODE_DEBUG
    vkt_debug_destroy_messenger(instance->vk_instance, instance->debug_messenger);
#endif

    vkDestroyInstance(instance->vk_instance, NULL);
}
