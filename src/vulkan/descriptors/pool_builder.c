#include "pool_builder.h"

#include "../common.h"

VktDescriptorPoolBuilder vkt_descriptor_pool_builder_new() {
    VktDescriptorPoolBuilder builder;
    LIST_HELPER_INIT_LIST(builder, VkDescriptorPoolSize, sizes, 4);
    builder.max_sets_count = 0;
    return builder;
}

void vkt_descriptor_pool_builder_push_size(VktDescriptorPoolBuilder *builder, VkDescriptorType type, uint32_t count) {
    VkDescriptorPoolSize pool_size;
    pool_size.type = type;
    pool_size.descriptorCount = count;
    LIST_HELPER_PUSH_ELEMENT(builder, VkDescriptorPoolSize, sizes, pool_size, 2);

    builder->max_sets_count += count;
}

int vkt_descriptor_pool_builder_build_pool(VktVulkanContext *context, VktDescriptorPoolBuilder *builder, VkDescriptorPool *pool) {
    VkDescriptorPoolCreateInfo pool_info;
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.pNext = NULL;

    pool_info.flags = 0;

    pool_info.maxSets = builder->max_sets_count;

    c_log(C_LOG_SEVERITY_DEBUG, "Descriptor Pool Builder :: Building descriptor pool with sizes (total = %lu):", builder->max_sets_count);
    for (size_t i = 0; i < builder->sizes_len; ++i) {
        VkDescriptorPoolSize *size = &builder->sizes[i];
        c_log(C_LOG_SEVERITY_DEBUG, "- Type: %s, count: %d", string_VkDescriptorType(size->type), size->descriptorCount);
    }
    printf("\n");

    pool_info.poolSizeCount = builder->sizes_len;
    pool_info.pPoolSizes = builder->sizes;

    VKT_CHECK(vkCreateDescriptorPool(context->logical_device.vk_device, &pool_info, NULL, pool));
    return VKT_GENERIC_SUCCESS;
}

void vkt_descriptor_pool_builder_destroy(VktDescriptorPoolBuilder *builder) {
    free(builder->sizes);
}
