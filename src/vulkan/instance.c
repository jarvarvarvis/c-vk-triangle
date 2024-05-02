#include "instance.h"
#include "common.h"

#include <stdint.h>

#include <c_log/c_log.h>

int vkt_create_vulkan_instance(VktVulkanInstance *instance, GLFWwindow *window) {
    memset(instance, 0, sizeof(VktVulkanInstance));

    // Get instance extensions
    uint32_t extension_count = 0;
    const char **extensions = glfwGetRequiredInstanceExtensions(&extension_count);
    if (!extensions) {
        c_log(C_LOG_SEVERITY_ERROR, "Unable to get required Vulkan instance extensions for glfw - Vulkan may not be supported by your device");
        return VKT_GENERIC_FAILURE;
    }

    // Application info
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

    // Instance create info
    VkInstanceCreateInfo inst_create_info;
    memset(&inst_create_info, 0, sizeof(VkInstanceCreateInfo));

    inst_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_create_info.pNext = NULL;
    inst_create_info.flags = 0;

    // Set application info
    inst_create_info.pApplicationInfo = &app_info;

    // No layers set for now
    inst_create_info.enabledLayerCount = 0;
    inst_create_info.ppEnabledLayerNames = NULL;

    // Set extensions
    inst_create_info.enabledExtensionCount = extension_count;
    inst_create_info.ppEnabledExtensionNames = extensions;

    // Create the instance
    VKT_CHECK(vkCreateInstance(&inst_create_info, NULL, &instance->vk_instance));
    return VKT_GENERIC_SUCCESS;
}

void vkt_delete_vulkan_instance(VktVulkanInstance *instance) {
    vkDestroyInstance(instance->vk_instance, NULL);
}
