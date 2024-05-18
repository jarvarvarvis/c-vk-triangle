#include "update_builder.h"

#include <vulkan/vk_enum_string_helper.h>

#include <c_log/c_log.h>

VktDescriptorSetUpdateBuilder vkt_descriptor_set_update_builder_new() {
    VktDescriptorSetUpdateBuilder builder;
    VKT_LIST_HELPER_INIT_LIST(builder, VkDescriptorBufferInfo, buffer_infos, 4);
    VKT_LIST_HELPER_INIT_LIST(builder, VkWriteDescriptorSet, set_writes, 4);

    VKT_LIST_HELPER_INIT_LIST(builder, VkCopyDescriptorSet, set_copies, 4);
    return builder;
}

void vkt_descriptor_set_update_builder_push_write(VktDescriptorSetUpdateBuilder *builder, VkWriteDescriptorSet write) {
    VKT_LIST_HELPER_PUSH_ELEMENT(builder, VkWriteDescriptorSet, set_writes, write, 2);
}

void vkt_descriptor_set_update_builder_push_buffer_write(VktDescriptorSetUpdateBuilder *builder, VkBuffer buffer, VkDeviceSize buffer_size, VkDescriptorSet set, uint32_t binding, VkDescriptorType descriptor_type) {
    // Create buffer info and push it into the list of buffer infos
    VkDescriptorBufferInfo buffer_info;
    buffer_info.buffer = buffer;
    buffer_info.offset = 0;
    buffer_info.range = buffer_size;

    VKT_LIST_HELPER_PUSH_ELEMENT(builder, VkDescriptorBufferInfo, buffer_infos, buffer_info, 2);

    // Set up information on how the descriptor set is updated
    VkWriteDescriptorSet write;
    memset(&write, 0, sizeof(VkWriteDescriptorSet));
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = NULL;

    write.dstBinding = binding;
    write.dstSet = set;

    write.descriptorCount = 1;

    write.descriptorType = descriptor_type;
    write.pBufferInfo = &builder->buffer_infos[builder->buffer_infos_len - 1]; // Point into the buffer_infos list

    vkt_descriptor_set_update_builder_push_write(builder, write);
}

void vkt_descriptor_set_update_builder_push_copy(VktDescriptorSetUpdateBuilder *builder, VkCopyDescriptorSet copy) {
    VKT_LIST_HELPER_PUSH_ELEMENT(builder, VkCopyDescriptorSet, set_copies, copy, 2);
}

void vkt_descriptor_set_update_builder_update_sets(VktVulkanContext *context, VktDescriptorSetUpdateBuilder *builder) {
    VkWriteDescriptorSet *set_writes_ptr = (builder->set_writes_len == 0) ? NULL : builder->set_writes;
    VkCopyDescriptorSet  *set_copies_ptr = (builder->set_copies_len == 0) ? NULL : builder->set_copies;

    c_log(C_LOG_SEVERITY_DEBUG, "Descriptor Set Update Builder :: Building descriptor set update with following writes and copies:");
    c_log(C_LOG_SEVERITY_DEBUG, "Writes (%lu total):", builder->set_writes_len);
    for (size_t i = 0; i < builder->set_writes_len; ++i) {
        VkWriteDescriptorSet *write = &set_writes_ptr[i];

        // This write has buffer information
        if (write->pBufferInfo) {
            c_log(C_LOG_SEVERITY_DEBUG, "- Descriptor %s (bound at %d) => buffer of size %lu",
                string_VkDescriptorType(write->descriptorType), write->dstBinding, write->pBufferInfo->range);
        }
    }
    c_log(C_LOG_SEVERITY_DEBUG, "Copies (%lu total):", builder->set_copies_len);
    for (size_t i = 0; i < builder->set_copies_len; ++i) {
        c_log(C_LOG_SEVERITY_DEBUG, "- Descriptor (bound at %d) => Descriptor (bound at %d)", set_copies_ptr[i].srcBinding, set_copies_ptr[i].dstBinding);
    }
    printf("\n");

    vkUpdateDescriptorSets(context->logical_device.vk_device, builder->set_writes_len, set_writes_ptr, builder->set_copies_len, set_copies_ptr);
}

void vkt_descriptor_set_update_builder_destroy(VktDescriptorSetUpdateBuilder *builder) {
    free(builder->buffer_infos);
    free(builder->set_writes);
    free(builder->set_copies);
}
