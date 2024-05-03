#include "debug.h"

#include <c_log/c_log.h>

static VKAPI_ATTR VkBool32 VKAPI_CALL vkt_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
	void *user_data
) {
	c_log(C_LOG_SEVERITY_DEBUG, "Vulkan Validation :: %s", callback_data->pMessage);
	return VK_FALSE;
}

VkDebugUtilsMessengerCreateInfoEXT vkt_debug_make_basic_messenger_create_info() {
    VkDebugUtilsMessengerCreateInfoEXT create_info;
    memset(&create_info, 0, sizeof(VkDebugUtilsMessengerCreateInfoEXT));

	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	create_info.flags = 0;
	create_info.pNext = NULL;
	create_info.messageSeverity =
	    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	create_info.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
	create_info.pfnUserCallback = vkt_debug_callback;

    return create_info;
}

VkResult vkt_create_debug_messenger(
    VkInstance instance,
	VkDebugUtilsMessengerEXT *debug_messenger
) {
	PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)
		vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != NULL) {
        VkDebugUtilsMessengerCreateInfoEXT create_info =
            vkt_debug_make_basic_messenger_create_info();
		return func(instance, &create_info, NULL, debug_messenger);
	}

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

VkResult vkt_destroy_debug_messenger(
    VkInstance instance,
	VkDebugUtilsMessengerEXT debug_messenger
) {
	PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != NULL) {
		func(instance, debug_messenger, NULL);
        return VK_SUCCESS;
	}

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}
