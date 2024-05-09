#include "module.h"

#include "../common.h"

int vkt_load_shader_module_from_file(VktVulkanContext *context, char *file_path, VkShaderModule *module) {
    // Open the file
    FILE *shader_file = fopen(file_path, "rb");
    if (!shader_file) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to open file '%s'!", file_path);
        return VKT_GENERIC_FAILURE;
    }

    // Get the length of the file
    fseek(shader_file, 0, SEEK_END);
    long shader_file_size = ftell(shader_file);

    // Read the file contents into a uint32_t buffer
    char *shader_buffer = malloc(shader_file_size * sizeof(char));
    rewind(shader_file);
    fread(shader_buffer, shader_file_size, 1, shader_file);

    // Close the file
    fclose(shader_file);

    // Create a new shader module using the loaded buffer
    VkShaderModuleCreateInfo create_info;

    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.pNext = NULL;

    create_info.flags = 0;

    create_info.codeSize = shader_file_size;
    create_info.pCode = (uint32_t*) shader_buffer;

    c_log(C_LOG_SEVERITY_DEBUG, "Creating shader module from file '%s' with size %lu", file_path, shader_file_size);

    int create_shader_module_result = vkCreateShaderModule(context->logical_device.vk_device, &create_info, NULL, module);
    free(shader_buffer);

    VKT_CHECK(create_shader_module_result);
    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_shader_module(VktVulkanContext *context, VkShaderModule module) {
    vkDestroyShaderModule(context->logical_device.vk_device, module, NULL);
}
