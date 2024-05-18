#include "layout_builder.h"

#include "../common.h"

VktDescriptorSetLayoutBuilder vkt_descriptor_set_layout_builder_new() {
    VktDescriptorSetLayoutBuilder builder;
    VKT_LIST_HELPER_INIT_LIST(builder, VkDescriptorSetLayoutBinding, bindings, 4);
    return builder;
}

void vkt_descriptor_set_layout_builder_push_layout_binding(VktDescriptorSetLayoutBuilder *builder, VkDescriptorSetLayoutBinding binding) {
    VKT_LIST_HELPER_PUSH_ELEMENT(builder, VkDescriptorSetLayoutBinding, bindings, binding, 2);
}

void vkt_descriptor_set_layout_builder_push_uniform_buffer_binding(VktDescriptorSetLayoutBuilder *builder, uint32_t binding_index, VkShaderStageFlags shader_stage) {
    VkDescriptorSetLayoutBinding binding;
    binding.binding = binding_index;
    binding.descriptorCount = 1;
    binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    binding.stageFlags = shader_stage;

    vkt_descriptor_set_layout_builder_push_layout_binding(builder, binding);
}

int vkt_descriptor_set_layout_builder_build_layout(VktVulkanContext *context, VktDescriptorSetLayoutBuilder *builder, VkDescriptorSetLayout *layout) {
    VkDescriptorSetLayoutCreateInfo set_info;
    set_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    set_info.pNext = NULL;

    set_info.flags = 0;

    c_log(C_LOG_SEVERITY_DEBUG, "Descriptor Set Layout Builder :: Building descriptor set layout with bindings:");
    for (size_t i = 0; i < builder->bindings_len; ++i) {
        VkDescriptorSetLayoutBinding *binding = &builder->bindings[i];
        c_log(C_LOG_SEVERITY_DEBUG, "- Type: %s, binding: %d", string_VkDescriptorType(binding->descriptorType), binding->binding);
    }
    printf("\n");

    set_info.bindingCount = builder->bindings_len;
    set_info.pBindings = builder->bindings;

    VKT_CHECK(vkCreateDescriptorSetLayout(context->logical_device.vk_device, &set_info, NULL, layout));
    return VKT_GENERIC_SUCCESS;
}

void vkt_descriptor_set_layout_builder_destroy(VktDescriptorSetLayoutBuilder *builder) {
    free(builder->bindings);
}
